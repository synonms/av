#include "FrameWrapper.h"

using namespace Synonms::Av::Clr::Enumerators;
using namespace Synonms::Av::Clr::Media;

FrameWrapper::FrameWrapper(redav::media::Frame* frame)
	: _frame(frame)
{
}

FrameWrapper::FrameWrapper(redav::media::Frame&& frame)
{
	_frame = new redav::media::Frame(std::move(frame));
}


redav::media::Frame* FrameWrapper::GetFrame()
{
	return _frame;
}

MediaType FrameWrapper::GetMediaType()
{
	return static_cast<MediaType>(_frame->GetMediaType());
}
