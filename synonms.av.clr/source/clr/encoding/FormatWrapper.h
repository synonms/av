#pragma once

#include <core\encoding\Format.h>

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
					FormatWrapper(synonms::av::encoding::Format* format);

					synonms::av::encoding::Format* GetCoreFormat();

				private:
					synonms::av::encoding::Format* _format;
				};
			}
		}
	}
}
