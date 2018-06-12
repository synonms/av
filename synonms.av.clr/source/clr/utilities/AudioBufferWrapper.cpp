#include "AudioBufferWrapper.h"

using namespace Synonms::Av::Clr::Utilities;

AudioBufferWrapper::AudioBufferWrapper(redav::utilities::AudioBuffer* audioBuffer)
	: _audioBuffer(audioBuffer)
{
}

redav::utilities::AudioBuffer* AudioBufferWrapper::GetCoreAudioBuffer()
{
	return _audioBuffer;
}
