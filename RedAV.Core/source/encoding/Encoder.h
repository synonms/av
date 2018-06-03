#pragma once

#include <functional>
#include <memory>

#include <encoding\Codec.h>
#include <enumerators\CodecType.h>
#include <media\Frame.h>
#include <media\Packet.h>
#include <utilities\AudioBuffer.h>
#include <utilities\Dictionary.h>
#include <utilities\RationalNumber.h>
#include <utilities\Resampler.h>

namespace redav
{
	namespace encoding
	{
		class Encoder
		{
		public:
			Encoder();
			~Encoder();

			void EncodeAudio(const utilities::AudioBuffer& buffer, const std::function<void(const media::Packet&)>& packetCompleteDelegate);
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
