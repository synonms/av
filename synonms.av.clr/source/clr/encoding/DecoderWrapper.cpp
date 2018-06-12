#include "DecoderWrapper.h"

using namespace Synonms::Av::Clr::Encoding;

DecoderWrapper::DecoderWrapper(redav::encoding::Decoder* decoder)
	: _decoder(decoder)
{
}

redav::encoding::Decoder* DecoderWrapper::GetCoreDecoder()
{
	return _decoder;
}

CodecParameters^ DecoderWrapper::GetAudioParameters()
{
	return gcnew CodecParameters(_decoder->GetAudioParameters());
}

CodecWrapper^ DecoderWrapper::GetCodec()
{
	return gcnew CodecWrapper(const_cast<redav::encoding::Codec*>(&_decoder->GetCodec()));
}
