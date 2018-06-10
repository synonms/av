#pragma once

#include <memory>

#include <encoding/Codec.h>
#include <media/Frame.h>

struct SwrContext;

namespace redav
{
	namespace utilities
	{
		class Resampler
		{
		public:
			Resampler();
			~Resampler();

			media::Frame Convert(const media::Frame& inputFrame);
			int Flush(const media::Frame& resampledFrame);
			void Initialise(const encoding::Codec& inCodec, const encoding::Codec& outCodec);

		public: //Internal
			SwrContext * GetSwrContext() const;

		private:
			class Implementation;
			std::unique_ptr<Implementation> implementation;
		};
	}
}