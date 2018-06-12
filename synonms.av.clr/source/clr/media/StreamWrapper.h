#pragma once

#include <media\Stream.h>

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
					StreamWrapper(redav::media::Stream* stream);

					redav::media::Stream* GetCoreStream();

				private:
					redav::media::Stream* _stream;
				};
			}
		}
	}
}
