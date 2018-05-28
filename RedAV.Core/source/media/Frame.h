#pragma once

#include <memory>

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

			enumerators::MediaType GetMediaType() const;
			utilities::RationalNumber GetTimeBase() const;
			void InitialiseForAudio(enumerators::SampleFormat sampleFormat, uint64_t channelLayout, int sampleRate, int noOfSamples);
			void InitialiseForVideo(enumerators::PixelFormat pixelFormat, int width, int height);
			bool IsValid() const;
			Frame& SetMediaType(enumerators::MediaType);
			Frame& SetTimeBase(const utilities::RationalNumber&);

		public: // Internal
			AVFrame* GetAVFrame() const;

		private:
			class Implementation;
			std::unique_ptr<Implementation> implementation;
		};
	}
}
