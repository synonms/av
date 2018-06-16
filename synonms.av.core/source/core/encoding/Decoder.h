#pragma once

#include <functional>
#include <memory>

#include <core\encoding\Codec.h>
#include <core\encoding\CodecParameters.h>
#include <core\enumerators\CodecType.h>
#include <core\media\Frame.h>
#include <core\media\Packet.h>
#include <core\media\Stream.h>
#include <core\utilities\RationalNumber.h>

namespace synonms
{
	namespace av
	{
		namespace encoding
		{
			class Decoder
			{
			public:
				Decoder();
				~Decoder();

				void DecodePacket(const media::Packet& packet, const std::function<void(const media::Frame&)>& frameCompleteDelegate);
				CodecParameters GetAudioParameters() const;
				const Codec& GetCodec() const;
				CodecParameters GetVideoParameters() const;
				void Initialise(enumerators::CodecType codecType);
				bool IsValid() const;
				void Open(CodecParameters* codecParameters);

			private:
				class Implementation;
				std::unique_ptr<Implementation> implementation;
			};
		}
	}
}
