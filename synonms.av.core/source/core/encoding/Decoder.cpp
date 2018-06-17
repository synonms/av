#include "Decoder.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include <iostream>

using namespace synonms::av::encoding;
using namespace synonms::av::enumerators;
using namespace synonms::av::media;
using namespace synonms::av::utilities;

class Decoder::Implementation 
{
public:
	Implementation()
	{
	}

	Codec codec;
	Frame decodedFrame;
};

Decoder::Decoder()
{
	implementation = std::make_unique<Implementation>();
}

Decoder::~Decoder()
{
}

void Decoder::DecodePacket(const Packet& packet, const std::function<void(const Frame&)>& frameCompleteDelegate)
{
#define MKTAG(a,b,c,d) ((a) | ((b) << 8) | ((c) << 16) | ((unsigned)(d) << 24))
#define FFERRTAG(a, b, c, d) (-(int)MKTAG(a, b, c, d))
#define AVERROR_EOF FFERRTAG( 'E','O','F',' ')
#define AVERROR(e) (-(e))   ///< Returns a negative error code from a POSIX error code, to return from library functions.

	if (!implementation->decodedFrame.IsValid()) throw std::exception("Decoder error: Frame not valid");

	// Send the packet with the compressed data to the decoder
	auto decodeResult = avcodec_send_packet(implementation->codec.GetCodecContext(), packet.GetAVPacket());

	if (decodeResult < 0) throw std::exception("Decoder error: Failed to send the packet");

	// Read all the output frames (in general there may be any number of them)
	while (true)
	{
		decodeResult = avcodec_receive_frame(implementation->codec.GetCodecContext(), implementation->decodedFrame.GetAVFrame());

		if (decodeResult == AVERROR(EAGAIN)) break; // Output is not available in this state
		if (decodeResult == AVERROR_EOF) break; // Decoder has been fully flushed and there will be no more

		if (decodeResult == AVERROR(EINVAL)) throw std::exception("Decoder error: CodecType not opened");
		if (decodeResult < 0) throw std::exception("Decoder error: Failed to receive frame");

		frameCompleteDelegate(implementation->decodedFrame);
	}
}

CodecParameters Decoder::GetAudioParameters() const
{
	return CodecParameters()
		.SetBitRate(implementation->codec.GetBitRate())
		.SetSampleRate(implementation->codec.GetSampleRate())
		.SetAudioChannelLayout(implementation->codec.GetAudioChannelLayout())
		.SetChannelCount(implementation->codec.GetChannelCount())
		.SetSampleFormat(implementation->codec.GetSampleFormat())
		.SetTimeBase(implementation->codec.GetTimeBase());
}

const Codec& Decoder::GetCodec() const
{
	return implementation->codec;
}

CodecParameters Decoder::GetVideoParameters() const
{
	return CodecParameters()
		.SetFrameRate(implementation->codec.GetFrameRate())
		.SetWidth(implementation->codec.GetWidth())
		.SetHeight(implementation->codec.GetHeight())
		.SetSampleAspectRatio(implementation->codec.GetSampleAspectRatio())
		.SetPixelFormat(implementation->codec.GetPixelFormat())
		.SetTimeBase(implementation->codec.GetTimeBase());
}

void Decoder::Initialise(CodecType codecType)
{
	std::cout << "Decoder: Initialising..." << std::endl;

	implementation->codec.Initialise(Codec::Purpose::Decoder, codecType);

	std::cout << "Decoder: Initialised" << std::endl;
}

bool Decoder::IsValid() const
{
	return implementation->codec.IsValid() && implementation->decodedFrame.IsValid();
}

void Decoder::Open(CodecParameters* codecParameters)
{
	implementation->codec.CopyParameters(codecParameters);
	implementation->codec.Open();

	implementation->decodedFrame.SetMediaType(implementation->codec.GetMediaType());
	implementation->decodedFrame.SetTimeBase(implementation->codec.GetTimeBase());
}
