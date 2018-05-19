#include "Muxer.h"

extern "C"
{
#include <libavformat/avformat.h>
}

#include <exception>
#include <fstream>

using namespace redav::enumerators;
using namespace redav::media;
using namespace redav::muxing;

void Muxer::Open(const std::string& filePath, AudioCodec audioCodecType, bool isInterleaved)
{
#define AVIO_FLAG_WRITE 2

	filePath_ = filePath;
	isInterleaved_ = isInterleaved;
	streams_.clear();

	const auto outputFormat = av_guess_format(nullptr, filePath.c_str(), nullptr);

	if (outputFormat == nullptr) throw std::exception("Muxer error: Failed to determine output format");

	formatContext_ = avformat_alloc_context();

	if (formatContext_ == nullptr) throw std::exception("Muxer error: Failed to create format context");

	formatContext_->oformat = outputFormat;

	const auto audioCodec = avcodec_find_decoder(AudioCodecMapper::ToFfmpeg(audioCodecType));

	if (audioCodec == nullptr) throw std::exception("Muxer error: Failed to find decoder");

	auto audioStream = avformat_new_stream(formatContext_, audioCodec);

	if (audioStream == nullptr) throw std::exception("Muxer error: Failed to create audio stream");

	streams_.emplace_back(audioStream);

	audioStream->codecpar->codec_id = AudioCodecMapper::ToFfmpeg(audioCodecType);
	audioStream->codecpar->codec_type = AVMEDIA_TYPE_AUDIO;
	audioStream->codecpar->codec_tag = 0;
	audioStream->codecpar->sample_rate = 48000;
	// audioStream_->pts = m_in_aud_strm->pts;
	// audioStream_->duration = m_in_aud_strm->duration;
	// audioStream_->time_base.num = m_in_aud_strm->time_base.num;
	// audioStream_->time_base.den = m_in_aud_strm->time_base.den;

	if (avio_open2(&formatContext_->pb, filePath.c_str(), AVIO_FLAG_WRITE, nullptr, nullptr) < 0) throw new std::exception("Muxer error: Unable to open output file");

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

void Muxer::WritePacket(Packet* packet) const
{
	if (isInterleaved_)
	{
		av_interleaved_write_frame(formatContext_, packet->GetPacket());
	}
	else
	{
		av_write_frame(formatContext_, packet->GetPacket());
	}
}