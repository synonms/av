#pragma once

#include <encoding\Codec.h>

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
					CodecWrapper(redav::encoding::Codec* codec);

					redav::encoding::Codec* GetCoreCodec();

				private:
					redav::encoding::Codec* _codec;
				};
			}
		}
	}
}
