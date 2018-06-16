#pragma once

#include <core\utilities\AudioBuffer.h>

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
					AudioBufferWrapper(synonms::av::utilities::AudioBuffer* audioBuffer);

					synonms::av::utilities::AudioBuffer* GetCoreAudioBuffer();

				private:
					synonms::av::utilities::AudioBuffer* _audioBuffer;
				};
			}
		}
	}
}
