#include "Frame.h"

extern "C"
{
#include <libavformat/avformat.h>
}

using namespace redav::encoding;
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

uint8_t* Frame::GetData() const
{
	return implementation->frame->data[0];
}

uint8_t** Frame::GetExtendedData() const
{
	return implementation->frame->extended_data;
}

MediaType Frame::GetMediaType() const
{
	return implementation->mediaType;
}

int Frame::GetNoOfSamples() const
{
	return implementation->frame->nb_samples;
}

uint64_t Frame::GetPresentationTimestamp() const
{
	return implementation->frame->pts;
}

RationalNumber Frame::GetTimeBase() const
{
	return implementation->timeBase;
}

void Frame::InitialiseForAudio(const Codec& codec)
{
	implementation->frame->format = SampleFormatMapper::ToFfmpeg(codec.GetSampleFormat());
	implementation->frame->channel_layout = codec.GetAudioChannelLayout();
	implementation->frame->sample_rate = codec.GetSampleRate();
	implementation->frame->nb_samples = codec.GetNoOfSamples();

	if (implementation->frame->nb_samples > 0) {
		if (av_frame_get_buffer(implementation->frame, 0) < 0) throw std::exception("Could not allocate audio buffer");
	}
}

void Frame::InitialiseForVideo(const Codec& codec)
{
	implementation->frame->format = PixelFormatMapper::ToFfmpeg(codec.GetPixelFormat());
	implementation->frame->width = codec.GetWidth();
	implementation->frame->height = codec.GetHeight();

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

Frame& Frame::SetPresentationTimestamp(uint64_t value)
{
	implementation->frame->pts = value;

	return *this;
}

Frame& Frame::SetTimeBase(const RationalNumber& timeBase)
{
	implementation->timeBase = timeBase;

	return *this;
}
