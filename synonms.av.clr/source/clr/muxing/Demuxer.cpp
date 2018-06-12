#include "Demuxer.h"

#include <clr\utilities\TypeHelper.h>

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace Synonms::Av::Clr::Encoding;
using namespace Synonms::Av::Clr::Enumerators;
using namespace Synonms::Av::Clr::Media;
using namespace Synonms::Av::Clr::Muxing;
using namespace Synonms::Av::Clr::Utilities;

Demuxer::Demuxer()
{
	_demuxer = new redav::muxing::Demuxer();
}

Demuxer::~Demuxer()
{
	this->!Demuxer();
}

Demuxer::!Demuxer()
{
	delete _demuxer;
}

void Demuxer::Close()
{
	_demuxer->Close();
}

void Demuxer::DecodePackets(MediaType mediaType)
{
	typedef void(__stdcall * nativeFunction)(const redav::media::Frame&);

	auto frameDecodedDelegate = gcnew FrameDecodedDelegate(this, &Demuxer::OnFrameDecoded);
	auto delegatePointer = Marshal::GetFunctionPointerForDelegate(frameDecodedDelegate);
	auto functionPointer = static_cast<nativeFunction>(delegatePointer.ToPointer());

	_demuxer->DecodePackets(static_cast<redav::enumerators::MediaType>(mediaType), functionPointer);
}

DecoderWrapper^ Demuxer::GetAudioDecoder()
{
	return gcnew DecoderWrapper(&_demuxer->GetAudioDecoder());
}

DecoderWrapper^ Demuxer::GetVideoDecoder()
{
	return gcnew DecoderWrapper(&_demuxer->GetVideoDecoder());
}

void Demuxer::Open(String^ filePath)
{
	_demuxer->Open(TypeHelper::ToNativeString(filePath));
}

void Demuxer::OnFrameDecoded(const redav::media::Frame& frame)
{
	auto frameWrapper = gcnew FrameWrapper(const_cast<redav::media::Frame*>(&frame));

	FrameDecodedEvent(frameWrapper);
}
