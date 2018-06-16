#include "CodecWrapper.h"

using namespace Synonms::Av::Clr::Encoding;

CodecWrapper::CodecWrapper(synonms::av::encoding::Codec* codec)
	: _codec(codec)
{
}

synonms::av::encoding::Codec* CodecWrapper::GetCoreCodec()
{
	return _codec;
}
