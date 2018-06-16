#pragma once

#include <core\encoding\Decoder.h>

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
					DecoderWrapper(synonms::av::encoding::Decoder* decoder);

					synonms::av::encoding::Decoder* GetCoreDecoder();

				private:
					synonms::av::encoding::Decoder* _decoder;
				};
			}
		}
	}
}
