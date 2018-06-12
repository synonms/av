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
#include <vector>

#include <encoding/Format.h>
#include <media/File.h>
#include <media/Stream.h>
#include <utilities/AudioBuffer.h>

using namespace redav::enumerators;
using namespace redav::encoding;
using namespace redav::media;
using namespace redav::muxing;
using namespace redav::utilities;

class Muxer::Implementation
{
public:
	bool isInterleaved{ true };
	File file;
	Format format;
	Stream audioStream;
	Stream videoStream;
	Encoder audioEncoder;
	Encoder videoEncoder;
	AudioBuffer audioBuffer;

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
		std::cout << "Muxer: Opening output file..." << std::endl;

		file.Open(filePath);

		std::cout << "Muxer: Initialising format..." << std::endl;

		format.Initialise();
		format.Open(file);

		std::cout << "Muxer: Format initialised" << std::endl;

		if (audioParameters != nullptr && audioParameters->GetCodecType() != CodecType::Unknown) SetUpAudio(audioParameters);
		if (videoParameters != nullptr && videoParameters->GetCodecType() != CodecType::Unknown) SetUpVideo(videoParameters);

		std::cout << "Muxer: Output file opened" << std::endl;
		std::cout << "Muxer: Writing header..." << std::endl;

		if (avformat_write_header(format.GetAVFormatContext(), nullptr) < 0) throw std::exception("Muxer error: Failed to write header");

		std::cout << "Muxer: Header written" << std::endl;
	}

	void SetUpAudio(CodecParameters* parameters)
	{
		std::cout << "Muxer: Initialising audio encoder..." << std::endl;

		audioEncoder.Initialise(parameters ? CodecType::PCM_Signed16BitLittleEndian : parameters->GetCodecType());

		audioEncoder.GetCodec()
			.SetSampleRate(parameters == nullptr ? 44100 : parameters->GetSampleRate())
			.SetBitRate(parameters == nullptr ? 64000 : parameters->GetBitRate())
			.SetAudioChannelLayout(parameters == nullptr ? AV_CH_LAYOUT_STEREO : parameters->GetAudioChannelLayout())
			.SetChannelCount(parameters == nullptr ? av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO) : parameters->GetChannelCount())
			.SetSampleFormat(parameters == nullptr ? SampleFormat::FloatPlanar : parameters->GetSampleFormat())
			.SetTimeBase(parameters == nullptr ? 1 : parameters->GetTimeBase());

		if (format.HasGlobalHeader()) audioEncoder.SetGlobalHeader();

		audioEncoder.Open(nullptr);

		std::cout << "Muxer: Audio encoder opened, initialising stream..." << std::endl;

		audioStream.Initialise(format);
		audioStream.SetTimeBase(audioEncoder.GetCodec().GetTimeBase());
		audioStream.CopyParameters(audioEncoder.GetCodec());

		std::cout << "Muxer: Stream initialised, initialising audio buffer..." << std::endl;

		audioBuffer.Initialise(audioEncoder.GetCodec().GetSampleFormat(), audioEncoder.GetCodec().GetChannelCount());

		std::cout << "Muxer: Audio setup complete" << std::endl;
	}

	void SetUpVideo(CodecParameters* parameters)
	{
		std::cout << "Muxer: Initialising video encoder..." << std::endl;

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

		std::cout << "Muxer: Video setup complete" << std::endl;
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

void Muxer::AddToBuffer(const Frame& frame)
{
//	std::cout << "o-> Muxer::WriteAudioFrame()" << std::endl;
//	std::cout << "Muxer: Adding " << frame.GetNoOfSamples() << " samples to buffer" << std::endl;

	auto noOfSamplesAdded = implementation->audioBuffer.AddSamples(frame.GetData(), frame.GetNoOfSamples());

	if (noOfSamplesAdded < frame.GetNoOfSamples()) throw new std::exception(("Muxer: Only " + std::to_string(noOfSamplesAdded) + " of " + std::to_string(frame.GetNoOfSamples()) + " added to buffer").c_str());
}

void Muxer::Close()
{
	std::cout << "Muxer: Writing trailer..." << std::endl;

	av_write_trailer(implementation->format.GetAVFormatContext());

	std::cout << "Muxer: Trailer written - closing..." << std::endl;

	implementation->TearDown();

	std::cout << "Muxer: Closed" << std::endl;
}

const Encoder& Muxer::GetAudioEncoder() const
{
	return implementation->audioEncoder;
}

const Encoder& Muxer::GetVideoEncoder() const
{
	return implementation->videoEncoder;
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

void Muxer::WriteBuffer()
{
	auto frameSize = implementation->audioEncoder.GetCodec().GetNoOfSamples();

	Frame outputFrame;
	outputFrame.InitialiseForAudio(implementation->audioEncoder.GetCodec(), frameSize);

	std::cout << "Muxer: Writing buffer..." << std::endl;

	while (implementation->audioBuffer.GetSize() >= 0)
	{
		auto noOfSamplesRead = implementation->audioBuffer.ReadSamples(outputFrame, frameSize);

		std::cout << "\rRequested " << frameSize << " and read " << noOfSamplesRead << " of " << implementation->audioBuffer.GetSize() << " samples from buffer";

		if (noOfSamplesRead < 0) throw std::exception("Muxer: Error reading samples from buffer");
		if (noOfSamplesRead == 0) break;

		outputFrame.SetNoOfSamples(noOfSamplesRead);

		implementation->audioEncoder.EncodeAudio(outputFrame, [=, &outputFrame](const Packet& packet)
		{
			if (implementation->WritePacket(outputFrame.GetTimeBase(), implementation->audioStream.GetStream(), packet.GetAVPacket()) < 0) throw std::exception("Muxer: Error writing audio packet");
		});
	}
}
