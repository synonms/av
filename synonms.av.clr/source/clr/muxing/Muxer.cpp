#include "Muxer.h"

#include <clr\utilities\TypeHelper.h>

using namespace System;
using namespace Synonms::Av::Clr::Encoding;
using namespace Synonms::Av::Clr::Media;
using namespace Synonms::Av::Clr::Muxing;
using namespace Synonms::Av::Clr::Utilities;

Muxer::Muxer()
{
	_muxer = new redav::muxing::Muxer();
}

Muxer::~Muxer()
{
	this->!Muxer();
}

Muxer::!Muxer()
{
	delete _muxer;
}

void Muxer::AddToBuffer(FrameWrapper^ frame)
{
	_muxer->AddToBuffer(*frame->GetFrame());
}

void Muxer::Close()
{
	_muxer->Close();
}

EncoderWrapper^ Muxer::GetAudioEncoder()
{
	return gcnew EncoderWrapper(const_cast<redav::encoding::Encoder*>(&_muxer->GetAudioEncoder()));
}

EncoderWrapper^ Muxer::GetVideoEncoder()
{
	return gcnew EncoderWrapper(const_cast<redav::encoding::Encoder*>(&_muxer->GetVideoEncoder()));
}

void Muxer::Open(String^ filePath, CodecParameters^ audioParameters, CodecParameters^ videoParameters)
{
	_muxer->Open(TypeHelper::ToNativeString(filePath), 
		audioParameters == nullptr ? nullptr : audioParameters->GetCoreCodecParameters(), 
		videoParameters == nullptr ? nullptr : videoParameters->GetCoreCodecParameters());
}

void Muxer::WriteBuffer()
{
	_muxer->WriteBuffer();
}
