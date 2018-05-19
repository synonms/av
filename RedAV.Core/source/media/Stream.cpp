#include "Stream.h"

extern "C"
{
#include <libavformat/avformat.h>
}

using namespace redav::enumerators;
using namespace redav::media;

Stream::Stream(AVStream* stream)
	: stream_(stream)
{
}

MediaType Stream::GetMediaType() const
{
	return MediaTypeMapper::FromFfmpeg(stream_->codecpar->codec_type);
}

AudioCodec Stream::GetAudioCodec() const
{
	return AudioCodecMapper::FromFfmpeg(stream_->codecpar->codec_id);
}
