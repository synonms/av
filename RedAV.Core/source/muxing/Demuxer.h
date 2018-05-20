#pragma once

#include <functional>
#include <map>
#include <memory>

#include <encoding\Decoder.h>
#include <media\Frame.h>
#include <media\Stream.h>

struct AVFormatContext;

namespace redav
{
	namespace muxing
	{
		class Demuxer
		{
			std::string filePath_;
			AVFormatContext* formatContext_{ nullptr };
			std::map<int, media::Stream> streams_;
			std::unique_ptr<encoding::Decoder> audioDecoder_;
			std::unique_ptr<encoding::Decoder> videoDecoder_;

		public:
			void Close();
			void DecodePackets(const std::function<void(media::Frame*)>& frameDecodedFunc) const;
			const std::map<int, media::Stream>& GetStreams() const;
			void Open(const std::string& filePath);
			encoding::Decoder* GetAudioDecoder() const;
			encoding::Decoder* GetVideoDecoder() const;
		};
	}
}
