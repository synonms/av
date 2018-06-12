#include "CodecParameters.h"

using namespace System;
using namespace Synonms::Av::Clr::Encoding;
using namespace Synonms::Av::Clr::Enumerators;
using namespace Synonms::Av::Clr::Utilities;

CodecParameters::CodecParameters()
{
	_codecParameters = new redav::encoding::CodecParameters();
}

CodecParameters::CodecParameters(const redav::encoding::CodecParameters& codecParameters)
{
	_codecParameters = new redav::encoding::CodecParameters(codecParameters);
}

CodecParameters::~CodecParameters()
{
	this->!CodecParameters();
}

CodecParameters::!CodecParameters()
{
	delete _codecParameters;
}

redav::encoding::CodecParameters* CodecParameters::GetCoreCodecParameters()
{
	return _codecParameters;
}

Int64 CodecParameters::GetBitRate()
{
	return _codecParameters->GetBitRate();
}

int CodecParameters::GetChannelCount()
{
	return _codecParameters->GetChannelCount();
}

UInt64 CodecParameters::GetAudioChannelLayout()
{
	return _codecParameters->GetAudioChannelLayout();
}

CodecType CodecParameters::GetCodecType()
{
	return static_cast<CodecType>(_codecParameters->GetCodecType());
}

int CodecParameters::GetHeight()
{
	return _codecParameters->GetHeight();
}

MediaType CodecParameters::GetMediaType()
{
	return static_cast<MediaType>(_codecParameters->GetMediaType());
}

PixelFormat CodecParameters::GetPixelFormat()
{
	return static_cast<PixelFormat>(_codecParameters->GetPixelFormat());
}

RationalNumber^ CodecParameters::GetSampleAspectRatio()
{
	return gcnew RationalNumber(_codecParameters->GetSampleAspectRatio());
}

SampleFormat CodecParameters::GetSampleFormat()
{
	return static_cast<SampleFormat>(_codecParameters->GetSampleFormat());
}

int CodecParameters::GetSampleRate()
{
	return _codecParameters->GetSampleRate();
}

RationalNumber^ CodecParameters::GetTimeBase()
{
	return gcnew RationalNumber(_codecParameters->GetTimeBase());
}

int CodecParameters::GetWidth()
{
	return _codecParameters->GetWidth();
}

CodecParameters^ CodecParameters::SetBitRate(Int64 value)
{
	_codecParameters->SetBitRate(value);

	return this;
}

CodecParameters^ CodecParameters::SetChannelCount(int value)
{
	_codecParameters->SetChannelCount(value);

	return this;
}

CodecParameters^ CodecParameters::SetAudioChannelLayout(UInt64 value)
{
	_codecParameters->SetAudioChannelLayout(value);

	return this;
}

CodecParameters^ CodecParameters::SetCodecType(CodecType value)
{
	_codecParameters->SetCodecType(static_cast<redav::enumerators::CodecType>(value));

	return this;
}

CodecParameters^ CodecParameters::SetHeight(int value)
{
	_codecParameters->SetHeight(value);

	return this;
}

CodecParameters^ CodecParameters::SetMediaType(MediaType value)
{
	_codecParameters->SetMediaType(static_cast<redav::enumerators::MediaType>(value));

	return this;
}

CodecParameters^ CodecParameters::SetPixelFormat(PixelFormat value)
{
	_codecParameters->SetPixelFormat(static_cast<redav::enumerators::PixelFormat>(value));

	return this;
}

CodecParameters^ CodecParameters::SetSampleAspectRatio(RationalNumber^ value)
{
	_codecParameters->SetSampleAspectRatio(value->GetCoreRationalNumber());

	return this;
}

CodecParameters^ CodecParameters::SetSampleFormat(SampleFormat value)
{
	_codecParameters->SetSampleFormat(static_cast<redav::enumerators::SampleFormat>(value));

	return this;
}

CodecParameters^ CodecParameters::SetSampleRate(int value)
{
	_codecParameters->SetSampleRate(value);

	return this;
}

CodecParameters^ CodecParameters::SetTimeBase(RationalNumber^ value)
{
	_codecParameters->SetTimeBase(value->GetCoreRationalNumber());

	return this;
}

CodecParameters^ CodecParameters::SetWidth(int value)
{
	_codecParameters->SetWidth(value);

	return this;
}
