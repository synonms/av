#pragma once

#include <memory>

#include <encoding\CodecParameters.h>
#include <enumerators\CodecType.h>
#include <enumerators\MediaType.h>
#include <enumerators\PixelFormat.h>
#include <enumerators\SampleFormat.h>
#include <utilities\RationalNumber.h>
#include <utilities\Dictionary.h>

struct AVCodec;
struct AVCodecContext;

namespace redav
{
	namespace encoding
	{
		class Codec
		{
		public:
			enum Purpose
			{
				Decoder,
				Encoder
			};

			Codec();
			~Codec();

			void CopyParameters(CodecParameters* parameters);
			void Initialise(Codec::Purpose purpose, enumerators::CodecType codecType);
			bool IsValid() const;
			void Open(utilities::Dictionary* options = nullptr);

		// Accessors
		public:
			uint64_t GetAudioChannelLayout() const;
			int64_t GetBitRate() const;
			int GetChannelCount() const;
			int GetFrameSize() const;
			int GetHeight() const;
			enumerators::MediaType GetMediaType() const;
			int GetNoOfSamples() const;
			enumerators::PixelFormat GetPixelFormat() const;
			utilities::RationalNumber GetSampleAspectRatio() const;
			enumerators::SampleFormat GetSampleFormat() const;
			int GetSampleRate() const;
			utilities::RationalNumber GetTimeBase() const;
			int GetWidth() const;

		// Mutators
		public:
			Codec& SetAudioChannelLayout(uint64_t value);
			Codec& SetBitRate(int64_t value);
			Codec& SetChannelCount(int value);
			Codec& SetFlag(int flag);
			Codec& SetHeight(int value);
			Codec& SetPixelFormat(enumerators::PixelFormat value);
			Codec& SetSampleAspectRatio(const utilities::RationalNumber& value);
			Codec& SetSampleFormat(enumerators::SampleFormat value);
			Codec& SetSampleRate(int value);
			Codec& SetTimeBase(const utilities::RationalNumber& value);
			Codec& SetWidth(int value);

		public: // Internal
			AVCodec* GetCodec() const;
			AVCodecContext* GetCodecContext() const;

		private:
			class Implementation;
			std::unique_ptr<Implementation> implementation;
		};
	}
}
