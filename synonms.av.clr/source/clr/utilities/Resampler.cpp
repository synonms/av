#include "Resampler.h"

using namespace Synonms::Av::Clr::Encoding;
using namespace Synonms::Av::Clr::Media;
using namespace Synonms::Av::Clr::Utilities;

Resampler::Resampler()
{
	_resampler = new synonms::av::utilities::Resampler();
}

Resampler::~Resampler()
{
	this->!Resampler();
}

Resampler::!Resampler()
{
	delete _resampler;
}

synonms::av::utilities::Resampler* Resampler::GetCoreResampler()
{
	return _resampler;
}

FrameWrapper^ Resampler::Convert(FrameWrapper^ inputFrame)
{
	return gcnew FrameWrapper(std::move(_resampler->Convert(*inputFrame->GetFrame())));
}

int Resampler::Flush(FrameWrapper^ resampledFrame)
{
	return _resampler->Flush(*resampledFrame->GetFrame());
}

void Resampler::Initialise(CodecWrapper^ inCodec, CodecWrapper^ outCodec)
{
	_resampler->Initialise(*inCodec->GetCoreCodec(), *outCodec->GetCoreCodec());
}
