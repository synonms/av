#pragma once

#include <cstdint>
#include <memory>

struct AVPacket;

namespace redav
{
	namespace media
	{
		class Packet
		{
		public:
			Packet();
			~Packet();

			uint8_t* GetData() const;
			uint64_t GetDuration() const;
			int64_t GetPosition() const;
			int GetSize() const;
			int GetStreamIndex() const;

		// Internal
		public:
			AVPacket* GetAVPacket() const;

		private:
			class Implementation;
			std::unique_ptr<Implementation> implementation;
		};
	}
}
