#pragma once

#include <media\Stream.h>

namespace RedAV
{
	namespace CLR
	{
		namespace Media
		{
			public ref class StreamWrapper
			{				
				const redav::media::Stream* stream_;

			internal:
				StreamWrapper(const redav::media::Stream* stream);

			public:
				redav::enumerators::AudioCodec GetAudioCodec();
				redav::enumerators::MediaType GetMediaType();
			};
		}
	}
}
