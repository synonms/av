#pragma once

#include <functional>
#include <memory>

#include <encoding\Codec.h>
#include <enumerators\CodecType.h>
#include <media\Frame.h>
#include <media\Packet.h>
#include <utilities\Dictionary.h>
#include <utilities\RationalNumber.h>

namespace redav
{
	namespace encoding
	{
		class Encoder
		{
		public:
			Encoder();
			~Encoder();

			void EncodeFrame(media::Frame* frame, const std::function<void(media::Packet*)>& packetCompleteDelegate);
			Codec& GetCodec() const;
			void Initialise(enumerators::CodecType codecType);
			void Open(utilities::Dictionary* options);
			void SetGlobalHeader();

		private:
			class Implementation;
			std::unique_ptr<Implementation> implementation;
		};
	}
}
