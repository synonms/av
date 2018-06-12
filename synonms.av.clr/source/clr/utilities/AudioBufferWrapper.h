#pragma once

#include <utilities\AudioBuffer.h>

namespace Synonms
{
	namespace Av
	{
		namespace Clr
		{
			namespace Utilities
			{
				public ref class AudioBufferWrapper
				{
				internal:
					AudioBufferWrapper(redav::utilities::AudioBuffer* audioBuffer);

					redav::utilities::AudioBuffer* GetCoreAudioBuffer();

				private:
					redav::utilities::AudioBuffer* _audioBuffer;
				};
			}
		}
	}
}
