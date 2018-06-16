#pragma once

#include <core\encoding\CodecParameters.h>

#include <clr\enumerators\CodecType.h>
#include <clr\enumerators\MediaType.h>
#include <clr\enumerators\PixelFormat.h>
#include <clr\enumerators\SampleFormat.h>
#include <clr\utilities\RationalNumber.h>

namespace Synonms
{
	namespace Av
	{
		namespace Clr
		{
			namespace Encoding
			{
				public ref class CodecParameters
				{
				public:
					CodecParameters();
					~CodecParameters();
					!CodecParameters();

					System::Int64 GetBitRate();
					int GetChannelCount();
					System::UInt64 GetAudioChannelLayout();
					Enumerators::CodecType GetCodecType();
					int GetHeight();
					Enumerators::MediaType GetMediaType();
					Enumerators::PixelFormat GetPixelFormat();
					Utilities::RationalNumber^ GetSampleAspectRatio();
					Enumerators::SampleFormat GetSampleFormat();
					int GetSampleRate();
					Utilities::RationalNumber^ GetTimeBase();
					int GetWidth();
					CodecParameters^ SetBitRate(System::Int64 value);
					CodecParameters^ SetChannelCount(int value);
					CodecParameters^ SetAudioChannelLayout(System::UInt64 value);
					CodecParameters^ SetCodecType(Enumerators::CodecType value);
					CodecParameters^ SetHeight(int value);
					CodecParameters^ SetMediaType(Enumerators::MediaType value);
					CodecParameters^ SetPixelFormat(Enumerators::PixelFormat value);
					CodecParameters^ SetSampleAspectRatio(Utilities::RationalNumber^ value);
					CodecParameters^ SetSampleFormat(Enumerators::SampleFormat value);
					CodecParameters^ SetSampleRate(int value);
					CodecParameters^ SetTimeBase(Utilities::RationalNumber^ value);
					CodecParameters^ SetWidth(int value);

				internal:
					CodecParameters(const synonms::av::encoding::CodecParameters& codecParameters);

					synonms::av::encoding::CodecParameters* GetCoreCodecParameters();

				private:
					synonms::av::encoding::CodecParameters* _codecParameters;
				};
			}
		}
	}
}
