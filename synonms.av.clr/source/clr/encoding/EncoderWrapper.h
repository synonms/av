#pragma once

#include <core\encoding\Encoder.h>

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
					EncoderWrapper(synonms::av::encoding::Encoder* encoder);

					synonms::av::encoding::Encoder* GetCoreEncoder();

				private:
					synonms::av::encoding::Encoder* _encoder;
				};
			}
		}
	}
}
