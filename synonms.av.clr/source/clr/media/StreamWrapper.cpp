#include "StreamWrapper.h"

using namespace Synonms::Av::Clr::Media;

StreamWrapper::StreamWrapper(synonms::av::media::Stream* stream)
	: _stream(stream)
{	
}

synonms::av::media::Stream* StreamWrapper::GetCoreStream()
{
	return _stream;
}
