#pragma once

#include <encoding\Decoder.h>

#include <clr\encoding\CodecParameters.h>
#include <clr\encoding\CodecWrapper.h>

namespace Synonms
{
	namespace Av
	{
		namespace Clr
		{
			namespace Encoding
			{
				public ref class DecoderWrapper
				{
				public:
					CodecParameters^ GetAudioParameters();
					CodecWrapper^ GetCodec();

				internal:
					DecoderWrapper(redav::encoding::Decoder* decoder);

					redav::encoding::Decoder* GetCoreDecoder();

				private:
					redav::encoding::Decoder* _decoder;
				};
			}
		}
	}
}
