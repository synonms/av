#include "Muxer.h"

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/opt.h>
}

#include <exception>
#include <fstream>
#include <iostream>

#include <encoding\Encoder.h>
#include <encoding\Format.h>
#include <media\Stream.h>

using namespace redav::enumerators;
using namespace redav::encoding;
using namespace redav::media;
using namespace redav::muxing;
using namespace redav::utilities;

class Muxer::Implementation
{
public:
	bool isInterleaved{ true };
	Format format;
	Stream audioStream;
	Stream videoStream;
	Encoder audioEncoder;
	Encoder videoEncoder;

	int WritePacket(const RationalNumber& timeBase, AVStream* stream, AVPacket* packet) const
	{
		// Rescale output packet timestamp values from codec to stream timebase 
		av_packet_rescale_ts(packet, timeBase.GetAVRational(), stream->time_base);

		packet->stream_index = stream->index;

		// Write the compressed frame to the media file
		if (isInterleaved)
		{
			return av_interleaved_write_frame(format.GetAVFormatContext(), packet);
		}

		return av_write_frame(format.GetAVFormatContext(), packet);
	}

	void SetUp(const std::string& filePath, CodecParameters* audioParameters, CodecParameters* videoParameters)
	{
#define AVIO_FLAG_WRITE 2
		std::cout << "Muxer: Initialising format..." << std::endl;

		format.Initialise(filePath);

		std::cout << "Muxer: Format initialised" << std::endl;

		if (audioParameters != nullptr && audioParameters->GetCodecType() != CodecType::Unknown) SetUpAudio(audioParameters);
		if (videoParameters != nullptr && videoParameters->GetCodecType() != CodecType::Unknown) SetUpVideo(videoParameters);

		std::cout << "Muxer: Opening output file..." << std::endl;

		if (avio_open2(&format.GetAVFormatContext()->pb, filePath.c_str(), AVIO_FLAG_WRITE, nullptr, nullptr) < 0) throw std::exception("Muxer error: Unable to open output file");

		std::cout << "Muxer: Output file opened" << std::endl;
		std::cout << "Muxer: Writing header..." << std::endl;

		if (avformat_write_header(format.GetAVFormatContext(), nullptr) < 0) throw std::exception("Muxer error: Failed to write header");

		std::cout << "Muxer: Header written" << std::endl;
	}

	void SetUpAudio(CodecParameters* parameters)
	{
		std::cout << "Muxer: Initialising audio stream..." << std::endl;

		audioStream.Initialise(format);

		std::cout << "Muxer: Audio stream initialised" << std::endl;
		std::cout << "Muxer: Initialising audio encoder..." << std::endl;

		audioEncoder.Initialise(parameters ? CodecType::PCM_Signed16BitLittleEndian : parameters->GetCodecType());

		std::cout << "Muxer: Audio encoder initialised" << std::endl;

		audioEncoder.GetCodec()
			.SetSampleRate(parameters == nullptr ? 44100 : parameters->GetSampleRate())
			.SetBitRate(parameters == nullptr ? 64000 : parameters->GetBitRate())
			.SetAudioChannelLayout(parameters == nullptr ? AV_CH_LAYOUT_STEREO : parameters->GetAudioChannelLayout())
			.SetChannelCount(parameters == nullptr ? av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO) : parameters->GetChannelCount())
			.SetSampleFormat(parameters == nullptr ? SampleFormat::FloatPlanar : parameters->GetSampleFormat())
			.SetTimeBase(parameters == nullptr ? 1 : parameters->GetTimeBase());

		std::cout << "Muxer: Audio encoder configured" << std::endl;

		audioEncoder.Open(nullptr);

		std::cout << "Muxer: Audio encoder opened" << std::endl;

		audioStream.CopyParameters(audioEncoder.GetCodec());

		std::cout << "Muxer: Audio parameters copied to stream" << std::endl;

		if (format.HasGlobalHeader()) audioEncoder.SetGlobalHeader();

		audioStream.SetTimeBase(audioEncoder.GetCodec().GetTimeBase());

		std::cout << "Muxer: Audio setup complete" << std::endl;
	}

	void SetUpVideo(CodecParameters* parameters)
	{
		videoStream.Initialise(format);

		// TODO - Add video codecs
		videoEncoder.Initialise(parameters ? CodecType::Unknown : parameters->GetCodecType());

		videoEncoder.GetCodec()
			.SetBitRate(parameters == nullptr ? 400000 : parameters->GetBitRate())
//			.SetVideoGopSize(parameters == nullptr ? 12 : parameters->GetGopSize())
			.SetHeight(parameters == nullptr ? 288 : parameters->GetHeight())
			.SetWidth(parameters == nullptr ? 352 : parameters->GetWidth())
			.SetSampleAspectRatio(parameters == nullptr ? RationalNumber(352 , 288) : parameters->GetSampleAspectRatio())
			.SetPixelFormat(parameters == nullptr ? PixelFormat::AV_PIX_FMT_YUV420P : parameters->GetPixelFormat())
			.SetTimeBase(parameters == nullptr ? 1 : parameters->GetTimeBase());

		videoEncoder.Open(nullptr);

		videoStream.CopyParameters(videoEncoder.GetCodec());

		if (format.HasGlobalHeader()) videoEncoder.SetGlobalHeader();

		videoStream.SetTimeBase(videoEncoder.GetCodec().GetTimeBase());
	}

	void TearDown()
	{
	}
};

Muxer::Muxer()
{
	implementation = std::make_unique<Implementation>();
}

Muxer::~Muxer()
{
	implementation->TearDown();
}

void Muxer::Close()
{
	std::cout << "Muxer: Writing trailer..." << std::endl;

	av_write_trailer(implementation->format.GetAVFormatContext());

	std::cout << "Muxer: Trailer written - closing..." << std::endl;

	implementation->TearDown();

	std::cout << "Muxer: Closed" << std::endl;
}

bool Muxer::IsValid() const
{
	return implementation->format.IsValid();
}

void Muxer::Open(const std::string& filePath, CodecParameters* audioParameters, CodecParameters* videoParameters)
{
	std::cout << "Muxer: Opening..." << std::endl;

	if (audioParameters != nullptr) std::cout << "Audio codec " << CodecTypeMapper::ToString(audioParameters->GetCodecType()) << " specified" << std::endl;
	if (videoParameters != nullptr) std::cout << "Video codec " << CodecTypeMapper::ToString(videoParameters->GetCodecType()) << " specified" << std::endl;

	implementation->SetUp(filePath, audioParameters, videoParameters);

	std::cout << "Muxer: Opened" << std::endl;
}

void Muxer::WriteFrame(Frame* frame)
{
	switch(frame->GetMediaType())
	{
	case MediaType::Audio:
		implementation->audioEncoder.EncodeFrame(frame, [=](Packet* packet)
		{
			if (implementation->WritePacket(frame->GetTimeBase(), implementation->audioStream.GetStream(), packet->GetPacket()) < 0) throw std::exception("Muxer: Error writing audio packet");
		});
		break;
	case MediaType::Video:
		implementation->videoEncoder.EncodeFrame(frame, [=](Packet* packet)
		{
			if (implementation->WritePacket(frame->GetTimeBase(), implementation->videoStream.GetStream(), packet->GetPacket()) < 0) throw std::exception("Muxer: Error writing video packet");
		});
		break;
	default:
		break;
	}
}

