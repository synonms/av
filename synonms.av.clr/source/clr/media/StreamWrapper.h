#pragma once

#include <core\media\Stream.h>

namespace Synonms
{
	namespace Av
	{
		namespace Clr
		{
			namespace Media
			{
				public ref class StreamWrapper
				{
				internal:
					StreamWrapper(synonms::av::media::Stream* stream);

					synonms::av::media::Stream* GetCoreStream();

				private:
					synonms::av::media::Stream* _stream;
				};
			}
		}
	}
}
