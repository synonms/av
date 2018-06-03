#pragma once

#pragma once

#include <memory>

#include <enumerators/SampleFormat.h>
#include <media/Frame.h>

struct AVAudioFifo;

namespace redav
{
	namespace utilities
	{
		class AudioBuffer
		{
		public:
			AudioBuffer();
			~AudioBuffer();

			void AddSamples(uint8_t** samples, int noOfSamples);
			int GetSize() const;
			void Initialise(enumerators::SampleFormat sampleFormat, int channelCount);
			void ReadSamples(media::Frame& outputFrame) const;

		public: //Internal
			AVAudioFifo* GetAVAudioFifo() const;

		private:
			class Implementation;
			std::unique_ptr<Implementation> implementation;
		};
	}
}