#include "Frame.h"

extern "C"
{
#include <libavformat/avformat.h>
}

using namespace redav::enumerators;
using namespace redav::media;
using namespace redav::utilities;

Frame::Frame(AVFrame* frame, MediaType mediaType, RationalNumber timeBase)
	: frame_(frame)
	, mediaType_(mediaType)
	, timeBase_(timeBase)
{}

AVFrame* Frame::GetFrame() const
{
	return frame_;
}

MediaType Frame::GetMediaType() const
{
	return mediaType_;
}

RationalNumber Frame::GetTimeBase() const
{
	return timeBase_;
}
