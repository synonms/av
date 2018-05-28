#pragma once

#include <functional>
#include <memory>

#include <encoding\Codec.h>
#include <encoding\CodecParameters.h>
#include <enumerators\CodecType.h>
#include <media\Frame.h>
#include <media\Packet.h>
#include <media\Stream.h>
#include <utilities\RationalNumber.h>

namespace redav
{
	namespace encoding
	{
		class Decoder
		{
		public:
			Decoder();
			~Decoder();

			void DecodePacket(media::Packet* packet, const std::function<void(media::Frame*)>& frameCompleteDelegate);
//			const Codec& GetCodec() const;
			CodecParameters GetAudioParameters() const;
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
