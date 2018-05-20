#include "Decoder.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

using namespace redav::encoding;
using namespace redav::enumerators;
using namespace redav::media;

Decoder::Decoder()
{
}

Decoder::~Decoder()
{
	if (codecContext_ != nullptr) avcodec_free_context(&codecContext_);
	if (decodedFrame_ != nullptr) av_frame_free(&decodedFrame_);
}

void Decoder::DecodePacket(Packet* packet, const std::function<void(Frame*)>& frameCompleteDelegate)
{
#define MKTAG(a,b,c,d) ((a) | ((b) << 8) | ((c) << 16) | ((unsigned)(d) << 24))
#define FFERRTAG(a, b, c, d) (-(int)MKTAG(a, b, c, d))
#define AVERROR_EOF FFERRTAG( 'E','O','F',' ')
#define AVERROR(e) (-(e))   ///< Returns a negative error code from a POSIX error code, to return from library functions.

	decodedFrame_ = decodedFrame_ == nullptr ? av_frame_alloc() : decodedFrame_;

	if (decodedFrame_ == nullptr) throw std::exception("Decoder error: Could not allocate frame");

	// Send the packet with the compressed data to the decoder
	auto decodeResult = avcodec_send_packet(codecContext_, packet->GetPacket());

	if (decodeResult < 0) throw std::exception("Decoder error: Failed to send the packet");

	// Read all the output frames (in general there may be any number of them)
	while (true)
	{
		decodeResult = avcodec_receive_frame(codecContext_, decodedFrame_);

		if (decodeResult == AVERROR(EAGAIN)) break; // Output is not available in this state
		if (decodeResult == AVERROR_EOF) break; // Decoder has been fully flushed and there will be no more

		if (decodeResult == AVERROR(EINVAL)) throw std::exception("Decoder error: Codec not opened");
		if (decodeResult < 0) throw std::exception("Decoder error: Failed to receive frame");

		Frame frame(decodedFrame_);

		frameCompleteDelegate(&frame);

		// const auto dataSize = av_get_bytes_per_sample(codecContext_->sample_fmt);
  //
		// if (dataSize < 0) throw std::exception("Decoder error: Failed to calculate data size");
  //
		// for (auto i = 0; i < decodedFrame_->nb_samples; i++)
		// {
		// 	for (auto ch = 0; ch < codecContext_->channels; ch++)
		// 	{
		// 		const auto dataStart = decodedFrame_->data[ch] + dataSize * i;
		// 		std::copy(dataStart, dataStart + dataSize, std::back_inserter(results));
		// 	}
		// }
	}
}

void Decoder::Open(AudioCodec audioCodec)
{
	SetCodecAndContext(AudioCodecMapper::ToFfmpeg(audioCodec));

	if (avcodec_open2(codecContext_, codec_, nullptr) < 0) throw std::exception("Decoder error: Failed to open codec");
}

void Decoder::Open(AVStream* stream)
{
	SetCodecAndContext(stream->codecpar->codec_id);

	if (avcodec_parameters_to_context(codecContext_, stream->codecpar) < 0) throw std::exception("Decoder error: Failed to copy parameters to context");

	if (avcodec_open2(codecContext_, codec_, nullptr) < 0) throw std::exception("Decoder error: Failed to open codec");
}

void Decoder::SetCodecAndContext(AVCodecID codecID)
{
	codec_ = codec_ == nullptr ? avcodec_find_decoder(codecID) : codec_;

	if (codec_ == nullptr) throw std::exception("Decoder error: Decoder not found");

	codecContext_ = codecContext_ == nullptr ? avcodec_alloc_context3(codec_) : codecContext_;

	if (codecContext_ == nullptr) throw std::exception("Decoder error: Failed to create codec context");
}