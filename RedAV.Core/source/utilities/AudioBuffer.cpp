#include "AudioBuffer.h"

extern "C"
{
#include <libavutil/audio_fifo.h>
}

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

void AudioBuffer::AddSamples(uint8_t** samples, int noOfSamples)
{
	// Make the FIFO as large as it needs to be to hold both, the old and the new samples.
	if (av_audio_fifo_realloc(implementation->audioFifo, av_audio_fifo_size(implementation->audioFifo) + noOfSamples) < 0) throw std::exception("AudioBuffer: Failed to resize audio buffer");

	// Store the new samples in the FIFO buffer.
	if (av_audio_fifo_write(implementation->audioFifo, (void**)samples, noOfSamples) < noOfSamples) throw new std::exception("AudioBuffer: Failed to store samples");
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

void AudioBuffer::ReadSamples(Frame& outputFrame) const
{
	if (av_audio_fifo_read(implementation->audioFifo, (void **)outputFrame.GetData(), outputFrame.GetNoOfSamples()) < outputFrame.GetNoOfSamples()) throw std::exception("AudioBuffer: Failed to read samples");
}

AVAudioFifo* AudioBuffer::GetAVAudioFifo() const
{
	return implementation->audioFifo;
}
