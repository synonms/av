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
#include <utilities/Resampler.h>

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

	CodecType codecType{ CodecType::Unknown };
	Codec codec;
	Frame encodedFrame;
	int64_t timestamp{0};

	void OpenAudio(Dictionary* options)
	{
		if (!encodedFrame.IsValid()) throw std::exception("Encoded audio frame is invalid");

		auto optionsCopy = options;

		codec.Open(optionsCopy);

		encodedFrame.InitialiseForAudio(codec);
	}

	void OpenVideo(Dictionary* options)
	{
		// TODO
	}
};

Encoder::Encoder()
{
	implementation = std::make_unique<Implementation>();
}

Encoder::~Encoder()
{
}

void Encoder::EncodeAudio(const AudioBuffer& buffer, const std::function<void(const Packet&)>& packetCompleteDelegate)
{
	// Temporary storage of the output samples of the frame written to the file.
	Frame tempFrame;
	tempFrame.InitialiseForAudio(implementation->codec);

	// Read as many samples from the FIFO buffer as required to fill the frame.
	// The samples are stored in the frame temporarily.
	buffer.ReadSamples(tempFrame);

	// Set a timestamp based on the sample rate for the container.
	tempFrame.SetPresentationTimestamp(implementation->timestamp);
	implementation->timestamp += tempFrame.GetNoOfSamples();

	// Send the frame with the uncompressed data to the encoder
	auto encodeResult = avcodec_send_frame(implementation->codec.GetCodecContext(), tempFrame.GetAVFrame());

	if (encodeResult < 0) throw std::exception("Encoder error: Failed to send the frame");

	Packet outputPacket;

	// Read all the output packets (in general there may be any number of them)
	while (true)
	{
		encodeResult = avcodec_receive_packet(implementation->codec.GetCodecContext(), outputPacket.GetAVPacket());

		if (encodeResult == AVERROR(EAGAIN)) break; // New input is required to return new output
		if (encodeResult == AVERROR_EOF) break; // Encoder has been fully flushed and there will be no more

		if (encodeResult == AVERROR(EINVAL)) throw std::exception("Encoder error: Codec not opened");
		if (encodeResult < 0) throw std::exception("Encoder error: Failed to receive packet");

		packetCompleteDelegate(outputPacket);
	}
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
