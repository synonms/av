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

using namespace redav::enumerators;
using namespace redav::media;
using namespace redav::muxing;
using namespace redav::utilities;

void Muxer::Open(const std::string& filePath, CodecType audioCodecType, bool isInterleaved)
{
#define AVIO_FLAG_WRITE 2

	filePath_ = filePath;
	isInterleaved_ = isInterleaved;
	streams_.clear();

	avformat_alloc_output_context2(&formatContext_, nullptr, nullptr, filePath.c_str());

	if (formatContext_ == nullptr)
	{
		// Could not deduce output format from file extension - default to mpeg
		avformat_alloc_output_context2(&formatContext_, nullptr, "mpeg", filePath.c_str());
	}
	
	if (formatContext_ == nullptr) throw std::exception("Muxer error: Failed to create format context");

	const auto outputFormat = formatContext_->oformat;

	if (outputFormat == nullptr) throw std::exception("Muxer error: Failed to determine output format");

	if (outputFormat->video_codec != AV_CODEC_ID_NONE)
	{
		videoStream_ = CreateStream(outputFormat->video_codec);
		videoEncoder_.Open(formatContext_, videoStream_, CodecTypeMapper::FromFfmpeg(outputFormat->video_codec), nullptr);
	}

	if (outputFormat->audio_codec != AV_CODEC_ID_NONE)
	{
		audioStream_ = CreateStream(outputFormat->audio_codec);
		audioEncoder_.Open(formatContext_, audioStream_, CodecTypeMapper::FromFfmpeg(outputFormat->audio_codec), nullptr);
	}

	if (avio_open2(&formatContext_->pb, filePath.c_str(), AVIO_FLAG_WRITE, nullptr, nullptr) < 0) throw std::exception("Muxer error: Unable to open output file");

	if (avformat_write_header(formatContext_, nullptr) < 0) throw std::exception("Muxer error: Failed to write header");
}

void Muxer::Close()
{
	filePath_.clear();
	streams_.clear();

	av_write_trailer(formatContext_);

	if (formatContext_ != nullptr) avformat_free_context(formatContext_);
}

void Muxer::WriteBytes(const std::vector<uint8_t>& data) const
{
	if (data.empty()) return;

	std::ofstream fileStream;
	fileStream.open(filePath_, std::ios_base::out | std::ios_base::binary | std::ios_base::app);

	fileStream.write(reinterpret_cast<const char*>(&data[0]), data.size());

	if (fileStream.is_open()) fileStream.close();
}

void Muxer::WriteFrame(Frame* frame)
{
	switch(frame->GetMediaType())
	{
	case MediaType::Audio:
		audioEncoder_.EncodeFrame(frame, [=](Packet* packet)
		{
			if (WritePacket(frame->GetTimeBase(), audioStream_, packet->GetPacket()) < 0) throw std::exception("Muxer: Error writing audio packet");
		});
		break;
	case MediaType::Video:
		videoEncoder_.EncodeFrame(frame, [=](Packet* packet)
		{
			if (WritePacket(frame->GetTimeBase(), videoStream_, packet->GetPacket()) < 0) throw std::exception("Muxer: Error writing video packet");
		});
		break;
	default:
		break;
	}
}



AVStream* Muxer::CreateStream(AVCodecID codecId) const
{
	const auto stream = avformat_new_stream(formatContext_, nullptr);

	if (stream == nullptr) throw std::exception("Could not allocate stream");

	stream->id = formatContext_->nb_streams - 1;

	return stream;
}



int Muxer::WritePacket(const RationalNumber& timeBase, AVStream* stream, AVPacket* packet) const
{
	// Rescale output packet timestamp values from codec to stream timebase 
	av_packet_rescale_ts(packet, AVRational{ timeBase.numerator, timeBase.denominator }, stream->time_base);

	packet->stream_index = stream->index;

	// Write the compressed frame to the media file
	if (isInterleaved_)
	{
		return av_interleaved_write_frame(formatContext_, packet);
	}

	return av_write_frame(formatContext_, packet);
}