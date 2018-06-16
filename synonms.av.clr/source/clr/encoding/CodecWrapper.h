#pragma once

#include <core\encoding\Codec.h>

namespace Synonms
{
	namespace Av
	{
		namespace Clr
		{
			namespace Encoding
			{
				public ref class CodecWrapper
				{
				internal:
					CodecWrapper(synonms::av::encoding::Codec* codec);

					synonms::av::encoding::Codec* GetCoreCodec();

				private:
					synonms::av::encoding::Codec* _codec;
				};
			}
		}
	}
}
