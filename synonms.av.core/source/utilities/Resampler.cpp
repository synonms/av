#include "Resampler.h"

extern "C"
{
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}

#include <enumerators/SampleFormat.h>

#include <iostream>

using namespace redav::enumerators;
using namespace redav::encoding;
using namespace redav::media;
using namespace redav::utilities;

class Resampler::Implementation
{
public:
	Implementation()
	{
		swrContext = swr_alloc();
	}

	SwrContext* swrContext{ nullptr };
	const Codec* inCodec;
	const Codec* outCodec;
};

Resampler::Resampler()
{
	implementation = std::make_unique<Implementation>();
}

Resampler::~Resampler()
{
	if (implementation->swrContext != nullptr)
	{
		swr_free(&implementation->swrContext);
		implementation->swrContext = nullptr;
	}
}

Frame Resampler::Convert(const Frame& inputFrame)
{
//	std::cout << "o-> Resampler::Convert()" << std::endl;

	if (implementation->inCodec == nullptr || implementation->outCodec == nullptr) throw std::exception("Resample: In/Out codecs not set");

//	std::cout << "Resampler: Input frame has " << inputFrame.GetNoOfSamples() << " samples" << std::endl;

	auto delay = swr_get_delay(implementation->swrContext, implementation->inCodec->GetSampleRate());

	// Calculate number of output samples
	int noOfOutputSamples = av_rescale_rnd(delay + inputFrame.GetNoOfSamples(), implementation->outCodec->GetSampleRate(), implementation->inCodec->GetSampleRate(), AV_ROUND_UP);

//	std::cout << "Resampler: " << noOfOutputSamples << " output samples expected" << std::endl;

	if (noOfOutputSamples == 0) throw std::exception("Resampler: Could not calculate no of output samples");

	Frame outputFrame;
	outputFrame.InitialiseForAudio(*implementation->outCodec, noOfOutputSamples);

	if (swr_convert(implementation->swrContext, outputFrame.GetExtendedData(), noOfOutputSamples, (const uint8_t**)inputFrame.GetExtendedData(), inputFrame.GetNoOfSamples()) < 0) throw std::exception("Resmapler: Failed to convert samples");

	return std::move(outputFrame);
}

void Resampler::Initialise(const Codec& inCodec, const Codec& outCodec)
{
	std::cout << "Resampler::Initialise()" << std::endl;

	if (implementation->swrContext == nullptr) throw std::exception("Resampler: Could not allocate context");

	implementation->inCodec = &inCodec;
	implementation->outCodec = &outCodec;

	av_opt_set_int(implementation->swrContext, "in_channel_count", inCodec.GetChannelCount(), 0);
	av_opt_set_int(implementation->swrContext, "in_sample_rate", inCodec.GetSampleRate(), 0);
	av_opt_set_sample_fmt(implementation->swrContext, "in_sample_fmt", SampleFormatMapper::ToFfmpeg(inCodec.GetSampleFormat()), 0);

	av_opt_set_int(implementation->swrContext, "out_channel_count", outCodec.GetChannelCount(), 0);
	av_opt_set_int(implementation->swrContext, "out_sample_rate", outCodec.GetSampleRate(), 0);
	av_opt_set_sample_fmt(implementation->swrContext, "out_sample_fmt", SampleFormatMapper::ToFfmpeg(outCodec.GetSampleFormat()), 0);

	if (swr_init(implementation->swrContext) < 0) throw std::exception("Resampler: Failed to initialise");
}

int Resampler::Flush(const Frame& resampledFrame)
{
	auto noOfSamples = swr_convert(implementation->swrContext, resampledFrame.GetExtendedData(), resampledFrame.GetNoOfSamples(), nullptr, 0);

	if (noOfSamples < 0) throw std::exception("Resampler: Failed to flush");

	return noOfSamples;
}

SwrContext* Resampler::GetSwrContext() const
{
	return implementation->swrContext;
}
