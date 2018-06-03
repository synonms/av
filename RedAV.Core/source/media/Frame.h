#pragma once

#include <memory>

#include <encoding/Codec.h>
#include <enumerators/MediaType.h>
#include <enumerators/PixelFormat.h>
#include <enumerators/SampleFormat.h>
#include <utilities/RationalNumber.h>

struct AVFrame;

namespace redav
{
	namespace media
	{
		class Frame
		{
		public:
			Frame();
			~Frame();

			uint8_t* GetData() const;
			uint8_t** GetExtendedData() const;
			enumerators::MediaType GetMediaType() const;
			int GetNoOfSamples() const;
			uint64_t GetPresentationTimestamp() const;
			utilities::RationalNumber GetTimeBase() const;
			void InitialiseForAudio(const encoding::Codec& codec);
			void InitialiseForVideo(const encoding::Codec& codec);
			bool IsValid() const;
			Frame& SetMediaType(enumerators::MediaType value);
			Frame& SetPresentationTimestamp(uint64_t value);
			Frame& SetTimeBase(const utilities::RationalNumber& value);

		public: // Internal
			AVFrame* GetAVFrame() const;

		private:
			class Implementation;
			std::unique_ptr<Implementation> implementation;
		};
	}
}
