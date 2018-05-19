#include "Demuxer.h"

extern "C"
{
#include <libavformat/avformat.h>
}

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
		streams_.emplace_back(formatContext_->streams[i]);
	}
}

void Demuxer::ForEachPacket(const std::function<void(Packet*)>& func) const
{
	AVPacket avPacket;
	Packet packet(&avPacket);

	while (av_read_frame(formatContext_, packet.GetPacket()) >= 0)
	{
		func(&packet);
	}
}

void Demuxer::Close()
{
	filePath_.clear();
	streams_.clear();

	if (formatContext_ != nullptr) avformat_close_input(&formatContext_);
}

const std::vector<Stream>& Demuxer::GetStreams() const
{
	return streams_;
}