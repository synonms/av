#pragma once

#include <core\encoding\Codec.h>
#include <core\encoding\Format.h>
#include <core\enumerators\CodecType.h>
#include <core\enumerators\MediaType.h>
#include <core\utilities\RationalNumber.h>

#include <memory>

struct AVStream;

namespace synonms
{
	namespace av
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
}
