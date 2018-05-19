#include "StreamWrapper.h"

using namespace RedAV::CLR::Media;
using namespace redav::enumerators;

StreamWrapper::StreamWrapper(const redav::media::Stream* stream)
	: stream_(stream)
{	
}

AudioCodec StreamWrapper::GetAudioCodec()
{
	return stream_->GetAudioCodec();
}

MediaType StreamWrapper::GetMediaType()
{
	return stream_->GetMediaType();
}
