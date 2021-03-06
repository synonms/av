#include "CodecParameters.h"

extern "C"
{
#include <libavcodec/avcodec.h>
}

using namespace synonms::av::encoding;
using namespace synonms::av::enumerators;
using namespace synonms::av::utilities;

class CodecParameters::Implementation
{
public:
	Implementation(AVCodecParameters* codecParameters)
		: codecParameters(codecParameters)
	{
		if (codecParameters == nullptr) SetUp();
	}

	AVCodecParameters* codecParameters{ nullptr };
	bool isOwned{ false };
	PixelFormat pixelFormat{ PixelFormat::Unknown };
	SampleFormat sampleFormat{ SampleFormat::None };
	RationalNumber timeBase{ 1, 24 };
	RationalNumber frameRate{ 24, 1 };
	int pictureGroupSize{ 10 };

	void SetUp()
	{
		codecParameters = avcodec_parameters_alloc();
		isOwned = true;
	}

	void TearDown()
	{
		if (codecParameters != nullptr && isOwned) {
			avcodec_parameters_free(&codecParameters);
			codecParameters = nullptr;
		}
	}
};

CodecParameters::CodecParameters()
	: CodecParameters(nullptr)
{
}

CodecParameters::CodecParameters(AVCodecParameters* codecParameters)
{
	implementation = std::make_unique<Implementation>(codecParameters);
}

CodecParameters::CodecParameters(const CodecParameters& other)
	: CodecParameters(nullptr)
{
	SetBitRate(other.GetBitRate());
	SetChannelCount(other.GetChannelCount());
	SetAudioChannelLayout(other.GetAudioChannelLayout());
	SetCodecType(other.GetCodecType());
	SetFrameRate(other.GetFrameRate());
	SetHeight(other.GetHeight());
	SetMediaType(other.GetMediaType());
	SetPixelFormat(other.GetPixelFormat());
	SetSampleAspectRatio(other.GetSampleAspectRatio());
	SetSampleFormat(other.GetSampleFormat());
	SetSampleRate(other.GetSampleRate());
	SetTimeBase(other.GetTimeBase());
	SetWidth(other.GetWidth());
}

CodecParameters& CodecParameters::operator=(const CodecParameters& other)
{
	SetBitRate(other.GetBitRate());
	SetChannelCount(other.GetChannelCount());
	SetAudioChannelLayout(other.GetAudioChannelLayout());
	SetCodecType(other.GetCodecType());
	SetFrameRate(other.GetFrameRate());
	SetHeight(other.GetHeight());
	SetMediaType(other.GetMediaType());
	SetPixelFormat(other.GetPixelFormat());
	SetSampleAspectRatio(other.GetSampleAspectRatio());
	SetSampleFormat(other.GetSampleFormat());
	SetSampleRate(other.GetSampleRate());
	SetTimeBase(other.GetTimeBase());
	SetWidth(other.GetWidth());

	return *this;
}

CodecParameters::~CodecParameters()
{
}

int64_t CodecParameters::GetBitRate() const
{
	return implementation->codecParameters->bit_rate;
}

int CodecParameters::GetChannelCount() const
{
	return implementation->codecParameters->channels;
}

uint64_t CodecParameters::GetAudioChannelLayout() const
{
	return implementation->codecParameters->channel_layout;
}

CodecType CodecParameters::GetCodecType() const
{
	return CodecTypeMapper::FromFfmpeg(implementation->codecParameters->codec_id);
}

RationalNumber CodecParameters::GetFrameRate() const
{
	return implementation->frameRate;
}

int CodecParameters::GetHeight() const
{
	return implementation->codecParameters->height;
}

MediaType CodecParameters::GetMediaType() const
{
	return MediaTypeMapper::FromFfmpeg(implementation->codecParameters->codec_type);
}

PixelFormat CodecParameters::GetPixelFormat() const
{
	return implementation->pixelFormat;
}

RationalNumber CodecParameters::GetSampleAspectRatio() const
{
	return { implementation->codecParameters->sample_aspect_ratio };
}

SampleFormat CodecParameters::GetSampleFormat() const
{
	return implementation->sampleFormat;
}

int CodecParameters::GetSampleRate() const
{
	return implementation->codecParameters->sample_rate;
}

RationalNumber CodecParameters::GetTimeBase() const
{
	return implementation->timeBase;
}

int CodecParameters::GetVideoPictureGroupSize() const
{
	return implementation->pictureGroupSize;
}

int CodecParameters::GetWidth() const
{
	return implementation->codecParameters->width;
}

bool CodecParameters::IsValid() const
{
	return implementation->codecParameters != nullptr;
}

CodecParameters& CodecParameters::SetBitRate(int64_t value)
{
	implementation->codecParameters->bit_rate = value;
	return *this;
}

CodecParameters& CodecParameters::SetChannelCount(int value)
{
	implementation->codecParameters->channels = value;
	return *this;
}

CodecParameters& CodecParameters::SetAudioChannelLayout(uint64_t value)
{
	implementation->codecParameters->channel_layout = value;
	return *this;
}

CodecParameters& CodecParameters::SetCodecType(CodecType value)
{
	implementation->codecParameters->codec_id = CodecTypeMapper::ToFfmpeg(value);
	return *this;
}

CodecParameters& CodecParameters::SetFrameRate(const RationalNumber& value)
{
	implementation->frameRate = value;
	return *this;
}

CodecParameters& CodecParameters::SetHeight(int value)
{
	implementation->codecParameters->height = value;
	return *this;
}

CodecParameters& CodecParameters::SetMediaType(MediaType value)
{
	implementation->codecParameters->codec_type = MediaTypeMapper::ToFfmpeg(value);
	return *this;
}

CodecParameters& CodecParameters::SetPixelFormat(PixelFormat value)
{
	implementation->pixelFormat = value;
	return *this;
}

CodecParameters& CodecParameters::SetSampleAspectRatio(const RationalNumber& value)
{
	implementation->codecParameters->sample_aspect_ratio = value.GetAVRational();
	return *this;
}

CodecParameters& CodecParameters::SetSampleFormat(SampleFormat value)
{
	implementation->sampleFormat = value;
	return *this;
}

CodecParameters& CodecParameters::SetSampleRate(int value)
{
	implementation->codecParameters->sample_rate = value;
	return *this;
}

CodecParameters& CodecParameters::SetTimeBase(const RationalNumber& value)
{
	implementation->timeBase = value;
	return *this;
}

CodecParameters& CodecParameters::SetVideoPictureGroupSize(int value)
{
	implementation->pictureGroupSize = value;
	return *this;
}

CodecParameters& CodecParameters::SetWidth(int value)
{
	implementation->codecParameters->width = value;
	return *this;
}

AVCodecParameters* CodecParameters::GetAVCodecParameters() const
{
	return implementation->codecParameters;
}
