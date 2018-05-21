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

CodecType Stream::GetAudioCodec() const
{
	return CodecTypeMapper::FromFfmpeg(stream_->codecpar->codec_id);
}
