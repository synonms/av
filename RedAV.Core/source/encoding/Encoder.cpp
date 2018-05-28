#include "Encoder.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/channel_layout.h>
#include <libswresample/swresample.h>
#include <libavutil/dict.h>
#include <libavutil/opt.h>
}

#include <media/Stream.h>

using namespace redav::encoding;
using namespace redav::enumerators;
using namespace redav::media;
using namespace redav::utilities;

class Encoder::Implementation
{
public:
	Implementation()
	{
	}

//	std::unique_ptr<Stream> stream{ nullptr };
	enumerators::CodecType codecType{ enumerators::CodecType::Unknown };
	Codec codec;
	Frame encodedFrame;
	Frame tempFrame;
	SwrContext* resampleContext{ nullptr };
	int sampleCount{ 0 };

	void OpenAudio(Dictionary* options)
	{
		if (!encodedFrame.IsValid()) throw std::exception("Encoded audio frame is invalid");

		auto optionsCopy = options;

		codec.Open(optionsCopy);

		encodedFrame.InitialiseForAudio(codec.GetSampleFormat(), codec.GetAudioChannelLayout(), codec.GetSampleRate(), codec.GetNoOfSamples());

		// Create resampler context
		resampleContext = swr_alloc();

		if (resampleContext == nullptr) throw std::exception("Could not allocate audio resampler context");

		av_opt_set_int(resampleContext, "in_channel_count", codec.GetChannelCount(), 0);
		av_opt_set_int(resampleContext, "in_sample_rate", codec.GetSampleRate(), 0);
		av_opt_set_sample_fmt(resampleContext, "in_sample_fmt", AV_SAMPLE_FMT_S16, 0);
		av_opt_set_int(resampleContext, "out_channel_count", codec.GetChannelCount(), 0);
		av_opt_set_int(resampleContext, "out_sample_rate", codec.GetSampleRate(), 0);
		av_opt_set_sample_fmt(resampleContext, "out_sample_fmt", SampleFormatMapper::ToFfmpeg(codec.GetSampleFormat()), 0);

		if (swr_init(resampleContext) < 0) throw std::exception("Failed to initialize the audio resampling context");
	}

	void OpenVideo(Dictionary* options)
	{
		auto optionsCopy = options;

		codec.Open(optionsCopy);

		encodedFrame.InitialiseForVideo(codec.GetPixelFormat(), codec.GetWidth(), codec.GetHeight());

		// If the output format is not YUV420P, then a temporary YUV420P picture is needed too. 
		// It is then converted to the required output format.
		if (codec.GetPixelFormat() != PixelFormat::AV_PIX_FMT_YUV420P) {
			tempFrame.InitialiseForVideo(PixelFormat::AV_PIX_FMT_YUV420P, codec.GetWidth(), codec.GetHeight());
		}
	}

	void PrepareAudioFrame(AVFrame* inputFrame)
	{
		if (inputFrame == nullptr) return;

		// Convert samples from native format to destination codec format, using the resampler
		// Compute destination number of samples
		const auto destinationNoOfSamples = av_rescale_rnd(swr_get_delay(resampleContext, codec.GetSampleRate()) + inputFrame->nb_samples, codec.GetSampleRate(), codec.GetSampleRate(), AV_ROUND_UP);

		if (destinationNoOfSamples != inputFrame->nb_samples) throw std::exception("Scaled sample count does not match audio frame samples");

		// When we pass a frame to the encoder, it may keep a reference to it internally - make sure we do not overwrite it here
		if (av_frame_make_writable(encodedFrame.GetAVFrame()) < 0) throw std::exception("Unable to make audio frame writable");

		// Convert to destination format
		if (swr_convert(resampleContext, encodedFrame.GetAVFrame()->data, destinationNoOfSamples, (const uint8_t **)inputFrame->data, inputFrame->nb_samples) < 0) throw std::exception("Error while converting audio frame");

		encodedFrame.GetAVFrame()->pts = av_rescale_q(sampleCount, { 1, codec.GetSampleRate() }, codec.GetTimeBase().GetAVRational() );

		sampleCount += destinationNoOfSamples;
	}

	void PrepareVideoFrame(AVFrame* inputFrame)
	{
		if (inputFrame == nullptr) return;

		// TODO
	}
};

Encoder::Encoder()
{
	implementation = std::make_unique<Implementation>();
}

Encoder::~Encoder()
{
	if (implementation->resampleContext != nullptr) swr_free(&implementation->resampleContext);
}

void Encoder::EncodeFrame(Frame* frame, const std::function<void(Packet*)>& packetCompleteDelegate)
{
#define MKTAG(a,b,c,d) ((a) | ((b) << 8) | ((c) << 16) | ((unsigned)(d) << 24))
#define FFERRTAG(a, b, c, d) (-(int)MKTAG(a, b, c, d))
#define AVERROR_EOF FFERRTAG( 'E','O','F',' ')
#define AVERROR(e) (-(e))   ///< Returns a negative error code from a POSIX error code, to return from library functions.

	if (frame == nullptr) return;

	const auto inputFrame = frame->GetAVFrame();

	if (inputFrame == nullptr) return;

	AVPacket encodedPacket = { 0 }; // data and size must be 0;
	av_init_packet(&encodedPacket);

	switch (implementation->codec.GetMediaType())
	{
	case MediaType::Audio:
		implementation->PrepareAudioFrame(inputFrame);
		break;
	case MediaType::Video:
		implementation->PrepareVideoFrame(inputFrame);
		break;
	default:
		break;
	}

	// Send the frame with the uncompressed data to the encoder
	auto encodeResult = avcodec_send_frame(implementation->codec.GetCodecContext(), inputFrame);

	if (encodeResult < 0) throw std::exception("Encoder error: Failed to send the frame");

	// Read all the output packets (in general there may be any number of them)
	while (true)
	{
		encodeResult = avcodec_receive_packet(implementation->codec.GetCodecContext(), &encodedPacket);

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

Codec& Encoder::GetCodec() const
{
	return implementation->codec;
}

void Encoder::Initialise(CodecType codecType)
{
	implementation->codec.Initialise(Codec::Purpose::Encoder, codecType);
}

void Encoder::Open(Dictionary* options)
{
	switch (implementation->codec.GetMediaType())
	{
	case MediaType::Audio:
		implementation->OpenAudio(options);
		break;
	case MediaType::Video:
		implementation->OpenVideo(options);
		break;
	default:
		break;
	}
}

void Encoder::SetGlobalHeader()
{
	/* Some formats want stream headers to be separate. */
	// #define AV_CODEC_FLAG_GLOBAL_HEADER (1 << 22)
	implementation->codec.SetFlag(1 << 22);
}
