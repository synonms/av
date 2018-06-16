#pragma once

#include <functional>
#include <memory>

#include <core\encoding\Codec.h>
#include <core\enumerators\CodecType.h>
#include <core\media\Frame.h>
#include <core\media\Packet.h>
#include <core\utilities\Dictionary.h>
#include <core\utilities\RationalNumber.h>

namespace synonms
{
	namespace av
	{
		namespace encoding
		{
			class Encoder
			{
			public:
				Encoder();
				~Encoder();

				void EncodeAudio(media::Frame& outputFrame, const std::function<void(const media::Packet&)>& packetCompleteDelegate);
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
}
