#pragma once

#include <enumerators/CodecType.h>
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

			enumerators::CodecType GetAudioCodec() const;
			enumerators::MediaType GetMediaType() const;

		private:
			AVStream* stream_;
		};
	}
}
