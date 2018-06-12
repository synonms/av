#pragma once

#include <encoding\Format.h>

namespace Synonms
{
	namespace Av
	{
		namespace Clr
		{
			namespace Encoding
			{
				public ref class FormatWrapper
				{
				internal:
					FormatWrapper(redav::encoding::Format* format);

					redav::encoding::Format* GetCoreFormat();

				private:
					redav::encoding::Format* _format;
				};
			}
		}
	}
}
