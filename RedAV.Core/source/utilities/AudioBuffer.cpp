#include "AudioBuffer.h"

extern "C"
{
#include <libavutil/audio_fifo.h>
}

#include <iostream>

using namespace redav::enumerators;
using namespace redav::media;
using namespace redav::utilities;

class AudioBuffer::Implementation
{
public:
	Implementation()
	{	
	}

	AVAudioFifo* audioFifo{ nullptr };
};

AudioBuffer::AudioBuffer()
{
	implementation = std::make_unique<Implementation>();
}

AudioBuffer::~AudioBuffer()
{

}

int AudioBuffer::AddSamples(uint8_t** samples, int noOfSamples)
{
//	std::cout << "o-> AudioBuffer::AddSamples - Pre-alloc FIFO size " << GetSize() << std::endl;

	// Make the FIFO as large as it needs to be to hold both the old and the new samples.
	if (av_audio_fifo_realloc(implementation->audioFifo, GetSize() + noOfSamples) < 0) throw std::exception("AudioBuffer: Failed to resize audio buffer");

//	std::cout << "AudioBuffer: Post-alloc FIFO size " << GetSize() << std::endl;

	// Store the new samples in the FIFO buffer.
	return av_audio_fifo_write(implementation->audioFifo, (void**)samples, noOfSamples);
}

int AudioBuffer::GetSize() const
{
	return av_audio_fifo_size(implementation->audioFifo);
}

void AudioBuffer::Initialise(SampleFormat sampleFormat, int channelCount)
{
	implementation->audioFifo = av_audio_fifo_alloc(SampleFormatMapper::ToFfmpeg(sampleFormat), channelCount, 1);

	if (implementation->audioFifo == nullptr) throw std::exception("AudioBuffer: Failed to allocate audio buffer");
}

int AudioBuffer::ReadSamples(Frame& outputFrame, int frameSize) const
{
//	std::cout << "o-> AudioBuffer::ReadSamples - Reading " << noOfSamplesToRead << " samples of " << GetSize() << "..." << std::endl;

	return av_audio_fifo_read(implementation->audioFifo, (void**)outputFrame.GetData(), frameSize);
}

AVAudioFifo* AudioBuffer::GetAVAudioFifo() const
{
	return implementation->audioFifo;
}
