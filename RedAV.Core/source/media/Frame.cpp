#include "Frame.h"

extern "C"
{
#include <libavformat/avformat.h>
}

using namespace redav::enumerators;
using namespace redav::media;
using namespace redav::utilities;

class Frame::Implementation 
{
public:
	Implementation()
	{
		frame = av_frame_alloc();
	}

	AVFrame* frame{ nullptr };
	enumerators::MediaType mediaType;
	utilities::RationalNumber timeBase;
};

Frame::Frame()
{
	implementation = std::make_unique<Implementation>();
}

Frame::~Frame()
{
	if (implementation->frame != nullptr) av_frame_free(&implementation->frame);
}

AVFrame* Frame::GetAVFrame() const
{
	return implementation->frame;
}

MediaType Frame::GetMediaType() const
{
	return implementation->mediaType;
}

RationalNumber Frame::GetTimeBase() const
{
	return implementation->timeBase;
}

void Frame::InitialiseForAudio(SampleFormat sampleFormat, uint64_t channelLayout, int sampleRate, int noOfSamples)
{
	implementation->frame->format = SampleFormatMapper::ToFfmpeg(sampleFormat);
	implementation->frame->channel_layout = channelLayout;
	implementation->frame->sample_rate = sampleRate;
	implementation->frame->nb_samples = noOfSamples;

	if (noOfSamples > 0) {
		if (av_frame_get_buffer(implementation->frame, 0) < 0) throw std::exception("Could not allocate audio buffer");
	}
}

void Frame::InitialiseForVideo(PixelFormat pixelFormat, int width, int height)
{
	implementation->frame->format = PixelFormatMapper::ToFfmpeg(pixelFormat);
	implementation->frame->width = width;
	implementation->frame->height = height;

	if (av_frame_get_buffer(implementation->frame, 32) < 0) throw std::exception("Could not allocate video buffer");
}

bool Frame::IsValid() const
{
	return implementation->frame != nullptr;
}

Frame& Frame::SetMediaType(MediaType mediaType)
{
	implementation->mediaType = mediaType;

	return *this;
}

Frame& Frame::SetTimeBase(const RationalNumber& timeBase)
{
	implementation->timeBase = timeBase;

	return *this;
}
