#include "Resampler.h"

extern "C"
{
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}

#include <enumerators/SampleFormat.h>

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

void Resampler::Convert(const Frame& inputFrame, uint8_t* outputBuffer)
{
	if (swr_convert(implementation->swrContext, &outputBuffer, inputFrame.GetNoOfSamples(), const_cast<const uint8_t**>(inputFrame.GetExtendedData()), inputFrame.GetNoOfSamples()) < 0) throw std::exception("Resampler: Failed to convert samples");
}

void Resampler::Initialise(const Codec& inCodec, const Codec& outCodec)
{
	if (implementation->swrContext == nullptr) throw std::exception("Resampler: Could not allocate context");

	av_opt_set_int(implementation->swrContext, "in_channel_count", inCodec.GetChannelCount(), 0);
	av_opt_set_int(implementation->swrContext, "in_sample_rate", inCodec.GetSampleRate(), 0);
	av_opt_set_sample_fmt(implementation->swrContext, "in_sample_fmt", SampleFormatMapper::ToFfmpeg(inCodec.GetSampleFormat()), 0);

	av_opt_set_int(implementation->swrContext, "out_channel_count", outCodec.GetChannelCount(), 0);
	av_opt_set_int(implementation->swrContext, "out_sample_rate", outCodec.GetSampleRate(), 0);
	av_opt_set_sample_fmt(implementation->swrContext, "out_sample_fmt", SampleFormatMapper::ToFfmpeg(outCodec.GetSampleFormat()), 0);

	if (swr_init(implementation->swrContext) < 0) throw std::exception("Resampler: Failed to initialise");
}

SwrContext* Resampler::GetSwrContext() const
{
	return implementation->swrContext;
}
