#include "PixelFormat.h"

extern "C"
{
#include <libavutil/avutil.h>
}

using namespace synonms::av::enumerators;

PixelFormat PixelFormatMapper::FromFfmpeg(AVPixelFormat value)
{
	switch (value)
	{
	case AV_PIX_FMT_YUV420P: return PixelFormat::AV_PIX_FMT_YUV420P;
	default: return PixelFormat::Unknown;
	}
}

AVPixelFormat PixelFormatMapper::ToFfmpeg(PixelFormat value)
{
	switch (value)
	{
	case PixelFormat::AV_PIX_FMT_YUV420P: return AV_PIX_FMT_YUV420P;
	default: return AV_PIX_FMT_NONE;
	}
}

std::string PixelFormatMapper::ToString(PixelFormat value)
{
	switch (value)
	{
	case PixelFormat::AV_PIX_FMT_YUV420P: return "YUV 420p";
	default: return "Unknown";
	}
}