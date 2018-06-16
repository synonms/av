#pragma once

#include <functional>
#include <memory>
#include <string>

#include <core\encoding\Decoder.h>
#include <core\enumerators\MediaType.h>
#include <core\media\Frame.h>

namespace synonms
{
	namespace av
	{
		namespace muxing
		{
			class Demuxer
			{
			public:
				Demuxer();
				~Demuxer();

				void Close();
				void DecodePackets(enumerators::MediaType mediaType, const std::function<void(const media::Frame&)>& frameDecodedFunc);
				encoding::Decoder& GetAudioDecoder() const;
				encoding::Decoder& GetVideoDecoder() const;
				bool IsValid() const;
				void Open(const std::string& filePath);

			private:
				class Implementation;
				std::unique_ptr<Implementation> implementation;
			};
		}
	}
}
