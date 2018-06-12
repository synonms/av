#pragma once

#include <utilities\Resampler.h>

#include <clr\encoding\CodecWrapper.h>
#include <clr\media\FrameWrapper.h>

namespace Synonms
{
	namespace Av
	{
		namespace Clr
		{
			namespace Utilities
			{
				public ref class Resampler
				{
				public:
					Resampler();
					~Resampler();
					!Resampler();

					Media::FrameWrapper^ Convert(Media::FrameWrapper^ inputFrame);
					int Flush(Media::FrameWrapper^ resampledFrame);
					void Initialise(Encoding::CodecWrapper^ inCodec, Encoding::CodecWrapper^ outCodec);

				internal:
					redav::utilities::Resampler* GetCoreResampler();

				private:
					redav::utilities::Resampler* _resampler;
				};
			}
		}
	}
}
