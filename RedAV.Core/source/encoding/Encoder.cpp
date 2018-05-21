#include "Encoder.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/channel_layout.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}

using namespace redav::encoding;
using namespace redav::enumerators;
using namespace redav::media;

Encoder::Encoder()
{
}

Encoder::~Encoder()
{
	if (codecContext_ != nullptr) avcodec_free_context(&codecContext_);
	if (encodedFrame_ != nullptr) av_frame_free(&encodedFrame_);
	if (tempFrame_ != nullptr) av_frame_free(&tempFrame_);
	if (resampleContext_ != nullptr) swr_free(&resampleContext_);
}

void Encoder::EncodeFrame(Frame* frame, const std::function<void(Packet*)>& packetCompleteDelegate)
{
#define MKTAG(a,b,c,d) ((a) | ((b) << 8) | ((c) << 16) | ((unsigned)(d) << 24))
#define FFERRTAG(a, b, c, d) (-(int)MKTAG(a, b, c, d))
#define AVERROR_EOF FFERRTAG( 'E','O','F',' ')
#define AVERROR(e) (-(e))   ///< Returns a negative error code from a POSIX error code, to return from library functions.

	if (frame == nullptr) return;

	const auto inputFrame = frame->GetFrame();

	if (inputFrame == nullptr) return;

	AVPacket encodedPacket = { 0 }; // data and size must be 0;
	av_init_packet(&encodedPacket);

	switch (codec_->type)
	{
	case AVMEDIA_TYPE_AUDIO:
		PrepareAudioFrame(inputFrame);
		break;
	case AVMEDIA_TYPE_VIDEO:
		PrepareVideoFrame(inputFrame);
		break;
	default:
		break;
	}

	// Send the frame with the uncompressed data to the encoder
	auto encodeResult = avcodec_send_frame(codecContext_, frame->GetFrame());

	if (encodeResult < 0) throw std::exception("Encoder error: Failed to send the frame");

	// Read all the output packets (in general there may be any number of them)
	while (true)
	{
		encodeResult = avcodec_receive_packet(codecContext_, &encodedPacket);

		if (encodeResult == AVERROR(EAGAIN)) break; // New input is required to return new output
		if (encodeResult == AVERROR_EOF) break; // Encoder has been fully flushed and there will be no more

		if (encodeResult == AVERROR(EINVAL)) throw std::exception("Encoder error: Codec not opened");
		if (encodeResult < 0) throw std::exception("Encoder error: Failed to receive packet");

		Packet packet(&encodedPacket);

		packetCompleteDelegate(&packet);
	}

	// Draining mode
	// encodeResult = avcodec_send_frame(codecContext_, nullptr);
	// while (true)
	// {
	// 	encodeResult = avcodec_receive_packet(codecContext_, encodedPacket_);
	// 	if (encodeResult == AVERROR_EOF) break; // Encoder has been fully flushed and there will be no more
	// }
	// avcodec_flush_buffers(codecContext_);
}

void Encoder::Open(AVFormatContext* formatContext, AVStream* stream, CodecType codecType, AVDictionary* options)
{
	stream_ = stream;
	codecType_ = codecType;

	codec_ = avcodec_find_encoder(CodecTypeMapper::ToFfmpeg(codecType_));

	if (codec_ == nullptr) throw std::exception(("Encoder error: Encoder not found for codec " + CodecTypeMapper::ToString(codecType)).c_str());

	codecContext_ = avcodec_alloc_context3(codec_);

	if (codecContext_ == nullptr) throw std::exception("Encoder error: Failed to create codec context");

	/* Some formats want stream headers to be separate. */
	// #define AVFMT_GLOBALHEADER 0x0040
	// #define AV_CODEC_FLAG_GLOBAL_HEADER (1 << 22)
	if (formatContext->oformat->flags & 0x0040) codecContext_->flags |= (1 << 22);

	switch (codec_->type)
	{
	case AVMEDIA_TYPE_AUDIO:
		UpdateAudioContext();
		OpenAudio(options);
		break;
	case AVMEDIA_TYPE_VIDEO:
		UpdateVideoContext();
		OpenVideo(options);
		break;
	default:
		break;
	}
}

void Encoder::UpdateAudioContext() const
{
	codecContext_->sample_fmt = codec_->sample_fmts ? codec_->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
	codecContext_->bit_rate = 64000;
	codecContext_->sample_rate = 44100;
	if (codec_->supported_samplerates) {
		codecContext_->sample_rate = codec_->supported_samplerates[0];
		for (auto i = 0; codec_->supported_samplerates[i]; i++) {
			if (codec_->supported_samplerates[i] == 44100) codecContext_->sample_rate = 44100;
		}
	}
	codecContext_->channels = av_get_channel_layout_nb_channels(codecContext_->channel_layout);
	codecContext_->channel_layout = AV_CH_LAYOUT_STEREO;
	if (codec_->channel_layouts) {
		codecContext_->channel_layout = codec_->channel_layouts[0];
		for (auto i = 0; codec_->channel_layouts[i]; i++) {
			if (codec_->channel_layouts[i] == AV_CH_LAYOUT_STEREO) codecContext_->channel_layout = AV_CH_LAYOUT_STEREO;
		}
	}
	codecContext_->channels = av_get_channel_layout_nb_channels(codecContext_->channel_layout);
	stream_->time_base = { 1, codecContext_->sample_rate };
}

void Encoder::UpdateVideoContext() const
{
	codecContext_->codec_id = CodecTypeMapper::ToFfmpeg(codecType_);
	codecContext_->bit_rate = 400000;
	// Resolution must be a multiple of two.
	codecContext_->width = 352;
	codecContext_->height = 288;
	// timebase: This is the fundamental unit of time (in seconds) in terms of which frame timestamps are represented. 
	// For fixed-fps content, timebase should be 1/framerate and timestamp increments should be identical to 1.
	stream_->time_base = { 1, 25 }; // 25 images/sec
	codecContext_->time_base = stream_->time_base;
	codecContext_->gop_size = 12; /* emit one intra frame every twelve frames at most */
	codecContext_->pix_fmt = AV_PIX_FMT_YUV420P;
	if (codecContext_->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
		/* just for testing, we also add B-frames */
		codecContext_->max_b_frames = 2;
	}
	if (codecContext_->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
		/* Needed to avoid using macroblocks in which some coeffs overflow.
		* This does not happen with normal video, it just happens here as
		* the motion of the chroma plane does not match the luma plane. */
		codecContext_->mb_decision = 2;
	}
}

void Encoder::OpenVideo(AVDictionary* options)
{
	AVDictionary* optionsCopy{ nullptr };
	av_dict_copy(&optionsCopy, options, 0);

	const auto openResult = avcodec_open2(codecContext_, codec_, &optionsCopy);

	av_dict_free(&optionsCopy);

	if (openResult < 0) throw std::exception("Could not open video codec");

	encodedFrame_ = CreateVideoFrame(codecContext_->pix_fmt, codecContext_->width, codecContext_->height);

	if (encodedFrame_ == nullptr) throw std::exception("Could not allocate video frame");

	// If the output format is not YUV420P, then a temporary YUV420P picture is needed too. 
	// It is then converted to the required output format.
	if (codecContext_->pix_fmt != AV_PIX_FMT_YUV420P) {
		tempFrame_ = CreateVideoFrame(AV_PIX_FMT_YUV420P, codecContext_->width, codecContext_->height);
		if (tempFrame_ == nullptr) throw std::exception("Could not allocate temporary video frame");
	}

	if (avcodec_parameters_from_context(stream_->codecpar, codecContext_) < 0) throw std::exception("Could not copy the stream parameters");
}

void Encoder::OpenAudio(AVDictionary* options)
{
#define AV_CODEC_CAP_VARIABLE_FRAME_SIZE (1 << 16)

	AVDictionary* optionsCopy{ nullptr };
	av_dict_copy(&optionsCopy, options, 0);

	const auto openResult = avcodec_open2(codecContext_, codec_, &optionsCopy);

	av_dict_free(&optionsCopy);

	if (openResult < 0) throw std::exception("Could not open audio codec");

	const auto noOfSamples = (codecContext_->codec->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE) ? 10000 : codecContext_->frame_size;

	encodedFrame_ = CreateAudioFrame(codecContext_->sample_fmt, codecContext_->channel_layout, codecContext_->sample_rate, noOfSamples);

	if (encodedFrame_ == nullptr) throw std::exception("Could not allocate audio frame");

	const auto audioTempFrame_ = CreateAudioFrame(AV_SAMPLE_FMT_S16, codecContext_->channel_layout, codecContext_->sample_rate, noOfSamples);

	if (audioTempFrame_ == nullptr) throw std::exception("Could not allocate temporary audio frame");

	// Copy the stream parameters to the muxer
	if (avcodec_parameters_from_context(stream_->codecpar, codecContext_) < 0) throw std::exception("Could not copy the stream parameters");

	// Create resampler context
	resampleContext_ = swr_alloc();

	if (resampleContext_ == nullptr) throw std::exception("Could not allocate audio resampler context");

	av_opt_set_int(resampleContext_, "in_channel_count", codecContext_->channels, 0);
	av_opt_set_int(resampleContext_, "in_sample_rate", codecContext_->sample_rate, 0);
	av_opt_set_sample_fmt(resampleContext_, "in_sample_fmt", AV_SAMPLE_FMT_S16, 0);
	av_opt_set_int(resampleContext_, "out_channel_count", codecContext_->channels, 0);
	av_opt_set_int(resampleContext_, "out_sample_rate", codecContext_->sample_rate, 0);
	av_opt_set_sample_fmt(resampleContext_, "out_sample_fmt", codecContext_->sample_fmt, 0);

	if (swr_init(resampleContext_) < 0) throw std::exception("Failed to initialize the audio resampling context");
}

AVFrame* Encoder::CreateVideoFrame(AVPixelFormat pixelFormat, int width, int height)
{
	const auto frame = av_frame_alloc();

	if (frame == nullptr) return nullptr;

	frame->format = pixelFormat;
	frame->width = width;
	frame->height = height;

	if (av_frame_get_buffer(frame, 32) < 0) throw std::exception("Could not allocate video buffer");

	return frame;
}

AVFrame* Encoder::CreateAudioFrame(AVSampleFormat sampleFormat, uint64_t channelLayout, int sampleRate, int noOfSamples)
{
	const auto frame = av_frame_alloc();

	if (frame == nullptr) return nullptr;

	frame->format = sampleFormat;
	frame->channel_layout = channelLayout;
	frame->sample_rate = sampleRate;
	frame->nb_samples = noOfSamples;

	if (noOfSamples > 0) {
		if (av_frame_get_buffer(frame, 0) < 0) throw std::exception("Could not allocate audio buffer");
	}

	return frame;
}

void Encoder::PrepareAudioFrame(AVFrame* inputFrame)
{
	if (inputFrame == nullptr) return;

	// Convert samples from native format to destination codec format, using the resampler
	// Compute destination number of samples
	const auto destinationNoOfSamples = av_rescale_rnd(swr_get_delay(resampleContext_, codecContext_->sample_rate) + inputFrame->nb_samples, codecContext_->sample_rate, codecContext_->sample_rate, AV_ROUND_UP);

	if (destinationNoOfSamples != inputFrame->nb_samples) throw std::exception("Scaled sample count does not match audio frame samples");

	// When we pass a frame to the encoder, it may keep a reference to it internally - make sure we do not overwrite it here
	if (av_frame_make_writable(encodedFrame_) < 0) throw std::exception("Unable to make audio frame writable");

	// Convert to destination format
	if (swr_convert(resampleContext_, encodedFrame_->data, destinationNoOfSamples, (const uint8_t **)inputFrame->data, inputFrame->nb_samples) < 0) throw std::exception("Error while converting audio frame");

	encodedFrame_->pts = av_rescale_q(sampleCount_, { 1, codecContext_->sample_rate }, codecContext_->time_base);

	sampleCount_ += destinationNoOfSamples;
}

void Encoder::PrepareVideoFrame(AVFrame* inputFrame)
{
	if (inputFrame == nullptr) return;

	// TODO
}





/* check that a given sample format is supported by the encoder */
bool Encoder::IsSampleFormatSupported(AVCodec* codec, AVSampleFormat format)
{
	auto supportedFormats = codec->sample_fmts;

	while (*supportedFormats != AV_SAMPLE_FMT_NONE) {
		if (*supportedFormats == format)
			return true;
		supportedFormats++;
	}

	return false;
}

/* just pick the highest supported samplerate */
int Encoder::SelectHighestSampleRate(const AVCodec* codec)
{
	const int *p;
	int best_samplerate = 0;

	if (!codec->supported_samplerates)
		return 44100;

	p = codec->supported_samplerates;
	while (*p) {
		if (!best_samplerate || abs(44100 - *p) < abs(44100 - best_samplerate))
			best_samplerate = *p;
		p++;
	}
	return best_samplerate;
}

/* select layout with the highest channel count */
uint64_t Encoder::SelectHighestChannelCount(const AVCodec* codec)
{
	const uint64_t *p;
	uint64_t best_ch_layout = 0;
	int best_nb_channels = 0;

	if (!codec->channel_layouts)
		return AV_CH_LAYOUT_STEREO;

	p = codec->channel_layouts;
	while (*p) {
		int nb_channels = av_get_channel_layout_nb_channels(*p);

		if (nb_channels > best_nb_channels) {
			best_ch_layout = *p;
			best_nb_channels = nb_channels;
		}
		p++;
	}
	return best_ch_layout;
}