#pragma once

#include <enumerators/MediaType.h>
#include <utilities/RationalNumber.h>

struct AVFrame;

namespace redav
{
	namespace media
	{
		class Frame
		{
		public:
			Frame(AVFrame* frame, enumerators::MediaType mediaType, utilities::RationalNumber timeBase);

			AVFrame* GetFrame() const;
			enumerators::MediaType GetMediaType() const;
			utilities::RationalNumber GetTimeBase() const;

		private:
			AVFrame* frame_;
			enumerators::MediaType mediaType_;
			utilities::RationalNumber timeBase_;
		};
	}
}
