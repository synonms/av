#include "AudioBufferWrapper.h"

using namespace Synonms::Av::Clr::Utilities;

AudioBufferWrapper::AudioBufferWrapper(synonms::av::utilities::AudioBuffer* audioBuffer)
	: _audioBuffer(audioBuffer)
{
}

synonms::av::utilities::AudioBuffer* AudioBufferWrapper::GetCoreAudioBuffer()
{
	return _audioBuffer;
}
