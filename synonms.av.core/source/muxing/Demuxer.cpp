#include "Demuxer.h"

extern "C"
{
#include <libavformat/avformat.h>
#include <libavutil/error.h>
}

#include <iostream>

using namespace redav::encoding;
using namespace redav::enumerators;
using namespace redav::media;
using namespace redav::muxing;

class Demuxer::Implementation 
{
public:
	Implementation()
	{
	}

	AVFormatContext* formatContext{ nullptr };
	Decoder audioDecoder;
	Decoder videoDecoder;

	void SetUp(const std::string& filePath)
	{
		if (avformat_open_input(&formatContext, filePath.c_str(), nullptr, nullptr) < 0) throw std::exception(("Demuxer error: Failed to open input file " + filePath).c_str());

		std::cout << "Demuxer: Input file opened" << std::endl;

		if (avformat_find_stream_info(formatContext, nullptr) < 0) throw std::exception("Demuxer error: Failed to find stream info");

		std::cout << "Demuxer: Stream info found" << std::endl;

		if (formatContext->nb_streams != 1)
		{
			std::cout << "Demuxer: WARNING - " << formatContext->nb_streams << " streams found" << std::endl;
		}

		const auto audioStreamIndex = av_find_best_stream(formatContext, AVMediaType::AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);

		if (audioStreamIndex >= 0)
		{
			std::cout << "Demuxer: Found audio stream index " << audioStreamIndex << std::endl;

			const auto audioStream = formatContext->streams[audioStreamIndex];
			const auto codecType = CodecTypeMapper::FromFfmpeg(audioStream->codecpar->codec_id);

			if (codecType != CodecType::Unknown)
			{
				CodecParameters codecParameters(audioStream->codecpar);
				audioDecoder.Initialise(codecType);
				audioDecoder.Open(&codecParameters);

				std::cout << "Demuxer: Audio stream and decoder ready" << std::endl;
			}
			else
			{
				std::cout << "Demuxer: Audio stream codec unknown - skipping" << std::endl;
			}
		}

		const auto videoStreamIndex = av_find_best_stream(formatContext, AVMediaType::AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);

		if (videoStreamIndex >= 0)
		{
			std::cout << "Demuxer: Found video stream index " << videoStreamIndex << std::endl;

			const auto videoStream = formatContext->streams[videoStreamIndex];
			const auto codecType = CodecTypeMapper::FromFfmpeg(videoStream->codecpar->codec_id);

			if (codecType != CodecType::Unknown)
			{
				CodecParameters codecParameters(videoStream->codecpar);
				videoDecoder.Initialise(CodecTypeMapper::FromFfmpeg(videoStream->codecpar->codec_id));
				videoDecoder.Open(&codecParameters);

				std::cout << "Demuxer: Video stream and decoder ready" << std::endl;
			}
			else
			{
				std::cout << "Demuxer: Video stream codec unknown - skipping" << std::endl;
			}
		}
	}

	void TearDown()
	{
		if (formatContext != nullptr)
		{
			avformat_close_input(&formatContext);
			formatContext = nullptr;
		}
	}
};

Demuxer::Demuxer()
{
	implementation = std::make_unique<Implementation>();
}

Demuxer::~Demuxer()
{
}

bool Demuxer::IsValid() const
{
	return implementation->formatContext != nullptr;
}

void Demuxer::Open(const std::string& filePath)
{
	std::cout << "Demuxer: Initialising..." << std::endl;

	implementation->SetUp(filePath);

	std::cout << "Demuxer: Initialised" << std::endl;
}

void Demuxer::DecodePackets(MediaType mediaType, const std::function<void(const Frame&)>& frameDecodedFunc)
{
	Packet packet;
	int error;

	while ((error = av_read_frame(implementation->formatContext, packet.GetAVPacket())) >= 0)
	{
		switch (implementation->formatContext->streams[packet.GetAVPacket()->stream_index]->codecpar->codec_type)
		{
		case AVMEDIA_TYPE_AUDIO:
			if (mediaType == MediaType::Audio || mediaType == MediaType::AudioAndVideo) implementation->audioDecoder.DecodePacket(packet, [&frameDecodedFunc](const Frame& frame) { frameDecodedFunc(frame); });
			break;
		case AVMEDIA_TYPE_VIDEO:
			if (mediaType == MediaType::Video || mediaType == MediaType::AudioAndVideo) implementation->videoDecoder.DecodePacket(packet, [&frameDecodedFunc](const Frame& frame) { frameDecodedFunc(frame); });
			break;
		default:
			break;
		}
	}

	if (error == AVERROR_EOF)
	{
		// TODO - At the end of the file, flush the decoder
	}
	else 
	{
		throw new std::exception("Demuxer: Error decoding packet");
	}
}

Decoder& Demuxer::GetAudioDecoder() const
{
	return implementation->audioDecoder;
}

Decoder& Demuxer::GetVideoDecoder() const
{
	return implementation->videoDecoder;
}

void Demuxer::Close()
{
	implementation->TearDown();
}
