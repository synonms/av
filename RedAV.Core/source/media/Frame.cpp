#include "Frame.h"

extern "C"
{
#include <libavformat/avformat.h>
}

using namespace redav::media;

Frame::Frame(AVFrame* frame)
	: frame_(frame)
{}

AVFrame* Frame::GetFrame() const
{
	return frame_;
}
