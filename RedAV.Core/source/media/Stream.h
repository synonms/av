#pragma once

#include <enumerators/AudioCodec.h>
#include <enumerators/MediaType.h>

struct AVStream;

namespace redav
{
	namespace media
	{
		class Stream
		{
		public:
			Stream(AVStream* stream = nullptr);

			enumerators::AudioCodec GetAudioCodec() const;
			enumerators::MediaType GetMediaType() const;

		private:
			AVStream* stream_;
		};
	}
}
