#pragma once

#include <memory>

#include <core\enumerators\CodecType.h>
#include <core\enumerators\MediaType.h>
#include <core\enumerators\PixelFormat.h>
#include <core\enumerators\SampleFormat.h>
#include <core\utilities\RationalNumber.h>

struct AVCodecParameters;

namespace synonms
{
	namespace av
	{
		namespace encoding
		{
			class CodecParameters
			{
			public:
				CodecParameters();
				CodecParameters(const CodecParameters& other);
				CodecParameters& operator=(const CodecParameters& other);
				~CodecParameters();

				int64_t GetBitRate() const;
				int GetChannelCount() const;
				uint64_t GetAudioChannelLayout() const;
				enumerators::CodecType GetCodecType() const;
				int GetHeight() const;
				enumerators::MediaType GetMediaType() const;
				enumerators::PixelFormat GetPixelFormat() const;
				utilities::RationalNumber GetSampleAspectRatio() const;
				enumerators::SampleFormat GetSampleFormat() const;
				int GetSampleRate() const;
				utilities::RationalNumber GetTimeBase() const;
				int GetWidth() const;
				bool IsValid() const;
				CodecParameters& SetBitRate(int64_t value);
				CodecParameters& SetChannelCount(int value);
				CodecParameters& SetAudioChannelLayout(uint64_t value);
				CodecParameters& SetCodecType(enumerators::CodecType value);
				CodecParameters& SetHeight(int value);
				CodecParameters& SetMediaType(enumerators::MediaType value);
				CodecParameters& SetPixelFormat(enumerators::PixelFormat value);
				CodecParameters& SetSampleAspectRatio(const utilities::RationalNumber& value);
				CodecParameters& SetSampleFormat(enumerators::SampleFormat value);
				CodecParameters& SetSampleRate(int value);
				CodecParameters& SetTimeBase(const utilities::RationalNumber& value);
				CodecParameters& SetWidth(int value);

			public: // Internal
				CodecParameters(AVCodecParameters* codecParameters);
				AVCodecParameters* GetAVCodecParameters() const;

			private:
				class Implementation;
				std::unique_ptr<Implementation> implementation;
			};
		}
	}
}
