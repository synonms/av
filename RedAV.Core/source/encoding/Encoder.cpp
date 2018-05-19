#include "Encoder.h"
#include <iostream>

extern "C"
{
#include <libavcodec/avcodec.h>
}

using namespace redav::encoding;
using namespace redav::enumerators;
using namespace redav::media;

Encoder::Encoder(AudioCodec audioCodec)
{
	audioCodec_ = audioCodec;
}

Encoder::~Encoder()
{
	if (codecContext_ != nullptr) avcodec_free_context(&codecContext_);
}

void Encoder::EncodeFrame(Frame* frame, const std::function<void(Packet*)>& packetCompleteDelegate)
{
#define MKTAG(a,b,c,d) ((a) | ((b) << 8) | ((c) << 16) | ((unsigned)(d) << 24))
#define FFERRTAG(a, b, c, d) (-(int)MKTAG(a, b, c, d))
#define AVERROR_EOF FFERRTAG( 'E','O','F',' ')
#define AVERROR(e) (-(e))   ///< Returns a negative error code from a POSIX error code, to return from library functions.

	encodedPacket_ = encodedPacket_ == nullptr ? av_packet_alloc() : encodedPacket_;

	if (encodedPacket_ == nullptr) throw std::exception("Encoder error: Could not allocate packet");

	// Send the frame with the uncompressed data to the encoder
	auto encodeResult = avcodec_send_frame(codecContext_, frame->GetFrame());

	if (encodeResult < 0) throw std::exception("Encoder error: Failed to send the frame");

	// Read all the output packets (in general there may be any number of them)
	while (true)
	{
		encodeResult = avcodec_receive_packet(codecContext_, encodedPacket_);

		if (encodeResult == AVERROR(EAGAIN)) break; // New input is required to return new output
		if (encodeResult == AVERROR_EOF) break; // Encoder has been fully flushed and there will be no more

		if (encodeResult == AVERROR(EINVAL)) throw std::exception("Encoder error: Codec not opened");
		if (encodeResult < 0) throw std::exception("Encoder error: Failed to receive packet");

		Packet packet(encodedPacket_);

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

void Encoder::Open()
{
	codec_ = codec_ == nullptr ? avcodec_find_encoder(AudioCodecMapper::ToFfmpeg(audioCodec_)) : codec_;

	if (codec_ == nullptr) throw std::exception("Encoder error: Encoder not found");

	codecContext_ = codecContext_ == nullptr ? avcodec_alloc_context3(codec_) : codecContext_;

	if (codecContext_ == nullptr) throw std::exception("Encoder error: Failed to create codec context");

	codecContext_->bit_rate = 44100;
	codecContext_->sample_fmt = AV_SAMPLE_FMT_S16;

	if(!IsSampleFormatSupported(codec_, codecContext_->sample_fmt)) throw std::exception("Encoder error: Sample format not supported");

	/* select other audio parameters supported by the encoder */
	codecContext_->sample_rate = SelectHighestSampleRate(codec_);
	codecContext_->channel_layout = SelectHighestChannelCount(codec_);
	codecContext_->channels = av_get_channel_layout_nb_channels(codecContext_->channel_layout);

	std::cout << "Bit Rate:" << codecContext_->bit_rate << std::endl;
	std::cout << "Channel Layout:" << codecContext_->channel_layout << std::endl;
	std::cout << "Channels:" << codecContext_->channels << std::endl;

	if (avcodec_open2(codecContext_, codec_, nullptr) < 0) throw std::exception("Encoder error: Failed to open codec");
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