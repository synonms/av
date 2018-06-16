#include "DecoderWrapper.h"

using namespace Synonms::Av::Clr::Encoding;

DecoderWrapper::DecoderWrapper(synonms::av::encoding::Decoder* decoder)
	: _decoder(decoder)
{
}

synonms::av::encoding::Decoder* DecoderWrapper::GetCoreDecoder()
{
	return _decoder;
}

CodecParameters^ DecoderWrapper::GetAudioParameters()
{
	return gcnew CodecParameters(_decoder->GetAudioParameters());
}

CodecWrapper^ DecoderWrapper::GetCodec()
{
	return gcnew CodecWrapper(const_cast<synonms::av::encoding::Codec*>(&_decoder->GetCodec()));
}
