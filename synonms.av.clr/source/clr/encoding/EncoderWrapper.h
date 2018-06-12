#pragma once

#include <encoding\Encoder.h>

#include <clr\encoding\CodecWrapper.h>

namespace Synonms
{
	namespace Av
	{
		namespace Clr
		{
			namespace Encoding
			{
				public ref class EncoderWrapper
				{
				public:
					CodecWrapper^ GetCodec();

				internal:
					EncoderWrapper(redav::encoding::Encoder* encoder);

					redav::encoding::Encoder* GetCoreEncoder();

				private:
					redav::encoding::Encoder* _encoder;
				};
			}
		}
	}
}
