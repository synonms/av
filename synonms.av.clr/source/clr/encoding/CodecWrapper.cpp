#include "CodecWrapper.h"

using namespace Synonms::Av::Clr::Encoding;

CodecWrapper::CodecWrapper(redav::encoding::Codec* codec)
	: _codec(codec)
{
}

redav::encoding::Codec* CodecWrapper::GetCoreCodec()
{
	return _codec;
}
