#pragma once

#include <vector>

#include <enumerators/AudioCodec.h>
#include <media/Packet.h>
#include <media/Stream.h>

struct AVFormatContext;

namespace redav
{
	namespace muxing
	{

		class Muxer
		{
			std::string filePath_;
			bool isInterleaved_{ false };
			std::vector<media::Stream> streams_;

			AVFormatContext* formatContext_{ nullptr };

		public:
			void Close();
			void Open(const std::string& filePath, enumerators::AudioCodec audioCodecType, bool isInterleaved);
			void WriteBytes(const std::vector<uint8_t>& data) const;
			void WritePacket(media::Packet* packet) const;
		};
	}
}