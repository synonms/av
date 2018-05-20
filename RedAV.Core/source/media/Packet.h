#pragma once

#include <cstdint>

struct AVPacket;

namespace redav
{
	namespace media
	{
		class Packet
		{
		public:
			Packet(AVPacket* packet);

			uint8_t* GetData() const;
			uint64_t GetDuration() const;
			AVPacket* GetPacket() const;
			int64_t GetPosition() const;
			int GetSize() const;
			int GetStreamIndex() const;

		private:
			AVPacket* packet_;
		};
	}
}
