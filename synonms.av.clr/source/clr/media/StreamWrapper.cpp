#include "StreamWrapper.h"

using namespace Synonms::Av::Clr::Media;

StreamWrapper::StreamWrapper(redav::media::Stream* stream)
	: _stream(stream)
{	
}

redav::media::Stream* StreamWrapper::GetCoreStream()
{
	return _stream;
}
