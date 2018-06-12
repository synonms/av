#include "EncoderWrapper.h"

using namespace Synonms::Av::Clr::Encoding;

EncoderWrapper::EncoderWrapper(redav::encoding::Encoder* encoder)
	: _encoder(encoder)
{
}

redav::encoding::Encoder* EncoderWrapper::GetCoreEncoder()
{
	return _encoder;
}

CodecWrapper^ EncoderWrapper::GetCodec()
{
	return gcnew CodecWrapper(const_cast<redav::encoding::Codec*>(&_encoder->GetCodec()));
}
