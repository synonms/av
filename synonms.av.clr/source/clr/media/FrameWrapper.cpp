#include "FrameWrapper.h"

using namespace Synonms::Av::Clr::Enumerators;
using namespace Synonms::Av::Clr::Media;

FrameWrapper::FrameWrapper(synonms::av::media::Frame* frame)
	: _frame(frame)
{
}

FrameWrapper::FrameWrapper(synonms::av::media::Frame&& frame)
{
	_frame = new synonms::av::media::Frame(std::move(frame));
}


synonms::av::media::Frame* FrameWrapper::GetFrame()
{
	return _frame;
}

MediaType FrameWrapper::GetMediaType()
{
	return static_cast<MediaType>(_frame->GetMediaType());
}
