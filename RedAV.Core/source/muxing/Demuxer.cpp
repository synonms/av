#include "Demuxer.h"

extern "C"
{
#include <libavformat/avformat.h>
}

#include <iostream>

using namespace redav::encoding;
using namespace redav::media;
using namespace redav::muxing;

void Demuxer::Open(const std::string& filePath)
{
	filePath_ = filePath;
	streams_.clear();

	if (avformat_open_input(&formatContext_, filePath_.c_str(), nullptr, nullptr) < 0) throw std::exception(("Demuxer error: Failed to open input file " + filePath_).c_str());

	if (avformat_find_stream_info(formatContext_, nullptr) < 0) throw std::exception("Demuxer error: Failed to find stream info");

	for (auto i = 0u; i < formatContext_->nb_streams; ++i)
	{
		streams_[i] = formatContext_->streams[i];
	}

	auto audioStreamIndex = av_find_best_stream(formatContext_, AVMediaType::AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
	
	if (audioStreamIndex >= 0)
	{
		audioDecoder_ = std::make_unique<Decoder>();
		audioDecoder_->Open(formatContext_->streams[audioStreamIndex]);
	}

	auto videoStreamIndex = av_find_best_stream(formatContext_, AVMediaType::AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);

	if (videoStreamIndex >= 0)
	{
		videoDecoder_ = std::make_unique<Decoder>();
		videoDecoder_->Open(formatContext_->streams[videoStreamIndex]);
	}
}

void Demuxer::DecodePackets(const std::function<void(Frame*)>& frameDecodedFunc) const
{
	AVPacket avPacket;
	Packet packet(&avPacket);

	while (av_read_frame(formatContext_, packet.GetPacket()) >= 0)
	{
		switch (formatContext_->streams[packet.GetPacket()->stream_index]->codecpar->codec_type)
		{
		case AVMediaType::AVMEDIA_TYPE_AUDIO:
			audioDecoder_->DecodePacket(&packet, [&frameDecodedFunc](Frame* frame) { frameDecodedFunc(frame); });
			break;
		case AVMediaType::AVMEDIA_TYPE_VIDEO:
			videoDecoder_->DecodePacket(&packet, [&frameDecodedFunc](Frame* frame) { frameDecodedFunc(frame); });
			break;
		}
	}
}

void Demuxer::Close()
{
	filePath_.clear();
	streams_.clear();

	if (formatContext_ != nullptr) avformat_close_input(&formatContext_);
}

const std::map<int, Stream>& Demuxer::GetStreams() const
{
	return streams_;
}

Decoder* Demuxer::GetAudioDecoder() const
{
	return audioDecoder_.get();
}

Decoder* Demuxer::GetVideoDecoder() const
{
	return videoDecoder_.get();
}
