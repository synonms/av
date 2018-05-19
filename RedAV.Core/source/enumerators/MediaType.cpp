#include "MediaType.h"

extern "C"
{
#include <libavutil/avutil.h>
}

using namespace redav::enumerators;

MediaType MediaTypeMapper::FromFfmpeg(AVMediaType value)
{
	switch (value)
	{
	case AVMEDIA_TYPE_VIDEO: return MediaType::Video;
	case AVMEDIA_TYPE_AUDIO: return MediaType::Audio;
	default: return MediaType::Unknown;
	}
}

AVMediaType MediaTypeMapper::ToFfmpeg(MediaType value)
{
	switch (value)
	{
	case MediaType::Video: return AVMEDIA_TYPE_VIDEO;
	case MediaType::Audio: return AVMEDIA_TYPE_AUDIO;
	default: return AVMEDIA_TYPE_UNKNOWN;
	}
}

std::string MediaTypeMapper::ToString(MediaType type)
{
	switch (type)
	{
	case MediaType::Video: return "Video";
	case MediaType::Audio: return "Audio";
	default: return "Unknown";
	}
}
