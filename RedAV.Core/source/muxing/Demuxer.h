#pragma once

#include <functional>
#include <memory>
#include <string>

#include <encoding\Decoder.h>
#include <enumerators\MediaType.h>
#include <media\Frame.h>

namespace redav
{
	namespace muxing
	{
		class Demuxer
		{
		public:
			Demuxer();
			~Demuxer();

			void Close();
			void DecodePackets(enumerators::MediaType mediaType, const std::function<void(media::Frame*)>& frameDecodedFunc);
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
