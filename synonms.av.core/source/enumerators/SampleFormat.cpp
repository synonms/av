#include "SampleFormat.h"

extern "C"
{
#include <libavcodec/avcodec.h>
}

using namespace redav::enumerators;

SampleFormat SampleFormatMapper::FromFfmpeg(AVSampleFormat value)
{
	switch (value)
	{
		case AV_SAMPLE_FMT_U8: return SampleFormat::Unsigned8Bit;
		case AV_SAMPLE_FMT_S16: return SampleFormat::Signed16Bit;
		case AV_SAMPLE_FMT_S32: return SampleFormat::Signed32Bit;
		case AV_SAMPLE_FMT_FLT: return SampleFormat::Float;
		case AV_SAMPLE_FMT_DBL: return SampleFormat::Double;
		case AV_SAMPLE_FMT_U8P: return SampleFormat::Unsigned8BitPlanar;
		case AV_SAMPLE_FMT_S16P: return SampleFormat::Signed16BitPlanar;
		case AV_SAMPLE_FMT_S32P: return SampleFormat::Signed32BitPlanar;
		case AV_SAMPLE_FMT_FLTP: return SampleFormat::FloatPlanar;
		case AV_SAMPLE_FMT_DBLP: return SampleFormat::DoublePlanar;
		case AV_SAMPLE_FMT_S64: return SampleFormat::Signed64Bit;
		case AV_SAMPLE_FMT_S64P: return SampleFormat::Signed64BitPlanar;
		default: return SampleFormat::None;
	}
}

AVSampleFormat SampleFormatMapper::ToFfmpeg(SampleFormat value)
{
	switch (value)
	{
	case SampleFormat::Unsigned8Bit: return AV_SAMPLE_FMT_U8;
	case SampleFormat::Signed16Bit: return AV_SAMPLE_FMT_S16;
	case SampleFormat::Signed32Bit: return AV_SAMPLE_FMT_S32;
	case SampleFormat::Float: return AV_SAMPLE_FMT_FLT;
	case SampleFormat::Double: return AV_SAMPLE_FMT_DBL;
	case SampleFormat::Unsigned8BitPlanar: return AV_SAMPLE_FMT_U8P;
	case SampleFormat::Signed16BitPlanar: return AV_SAMPLE_FMT_S16P;
	case SampleFormat::Signed32BitPlanar: return AV_SAMPLE_FMT_S32P;
	case SampleFormat::FloatPlanar: return AV_SAMPLE_FMT_FLTP;
	case SampleFormat::DoublePlanar: return AV_SAMPLE_FMT_DBLP;
	case SampleFormat::Signed64Bit: return AV_SAMPLE_FMT_S64;
	case SampleFormat::Signed64BitPlanar: return AV_SAMPLE_FMT_S64P;
	default: return AV_SAMPLE_FMT_NONE;
	}
}

std::string SampleFormatMapper::ToString(SampleFormat value)
{
	switch (value)
	{
	case SampleFormat::Unsigned8Bit: return "Unsigned 8 bit";
	case SampleFormat::Signed16Bit: return "Signed 16 bit";
	case SampleFormat::Signed32Bit: return "Signed 32 bit";
	case SampleFormat::Float: return "Float";
	case SampleFormat::Double: return "Double";
	case SampleFormat::Unsigned8BitPlanar: return "Unsigned 8 bit (Planar)";
	case SampleFormat::Signed16BitPlanar: return "Signed 16 bit (Planar)";
	case SampleFormat::Signed32BitPlanar: return "Signed 32 bit (Planar)";
	case SampleFormat::FloatPlanar: return "Float (Planar)";
	case SampleFormat::DoublePlanar: return "Double (Planar)";
	case SampleFormat::Signed64Bit: return "Signed 64 bit";
	case SampleFormat::Signed64BitPlanar: return "Signed 64 bit (Planar)";
	default: return "Unknown";
	}
}