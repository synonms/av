#include "EncoderWrapper.h"

using namespace Synonms::Av::Clr::Encoding;

EncoderWrapper::EncoderWrapper(synonms::av::encoding::Encoder* encoder)
	: _encoder(encoder)
{
}

synonms::av::encoding::Encoder* EncoderWrapper::GetCoreEncoder()
{
	return _encoder;
}

CodecWrapper^ EncoderWrapper::GetCodec()
{
	return gcnew CodecWrapper(const_cast<synonms::av::encoding::Codec*>(&_encoder->GetCodec()));
}
