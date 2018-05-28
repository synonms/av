#pragma once

#include <encoding\Codec.h>
#include <encoding\Format.h>
#include <enumerators\CodecType.h>
#include <enumerators\MediaType.h>
#include <utilities\RationalNumber.h>

#include <memory>

struct AVStream;

namespace redav
{
	namespace media
	{
		class Stream
		{
		public:
			Stream();
			~Stream();

			void CopyParameters(const encoding::Codec& codec);
			enumerators::CodecType GetAudioCodecType() const;
			enumerators::MediaType GetMediaType() const;
			void Initialise(const encoding::Format& format);
			bool IsValid() const;
			void SetTimeBase(utilities::RationalNumber timeBase);

		// Internal
		public:
			AVStream * GetStream() const;

		private:
			class Implementation;
			std::unique_ptr<Implementation> implementation;
		};
	}
}
