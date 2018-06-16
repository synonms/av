#pragma once

#pragma once

#include <memory>

#include <core\enumerators\SampleFormat.h>
#include <core\media\Frame.h>

struct AVAudioFifo;

namespace synonms
{
	namespace av
	{
		namespace utilities
		{
			class AudioBuffer
			{
			public:
				AudioBuffer();
				~AudioBuffer();

				int AddSamples(uint8_t** samples, int noOfSamples);
				int GetSize() const;
				void Initialise(enumerators::SampleFormat sampleFormat, int channelCount);
				int ReadSamples(media::Frame& outputFrame, int frameSize) const;

			public: //Internal
				AVAudioFifo * GetAVAudioFifo() const;

			private:
				class Implementation;
				std::unique_ptr<Implementation> implementation;
			};
		}
	}
}