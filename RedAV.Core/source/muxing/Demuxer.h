#pragma once

#include <functional>
#include <vector>

#include <media/Packet.h>
#include <media/Stream.h>

struct AVFormatContext;

namespace redav
{
	namespace muxing
	{
		class Demuxer
		{
			std::string filePath_;
			AVFormatContext* formatContext_{ nullptr };
			std::vector<media::Stream> streams_;

		public:

			void Close();
			void ForEachPacket(const std::function<void(media::Packet*)>& func) const;
			const std::vector<media::Stream>& GetStreams() const;
			void Open(const std::string& filePath);
		};
	}
}
