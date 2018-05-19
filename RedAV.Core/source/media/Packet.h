#pragma once

struct AVPacket;

namespace redav
{
	namespace media
	{
		class Packet
		{
		public:
			Packet(AVPacket* packet);

			AVPacket* GetPacket() const;

		private:
			AVPacket* packet_;
		};
	}
}
