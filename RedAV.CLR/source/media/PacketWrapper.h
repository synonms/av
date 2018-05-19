#pragma once

#include <media\Packet.h>

namespace RedAV
{
	namespace CLR
	{
		namespace Media
		{
			public ref class PacketWrapper
			{
				redav::media::Packet* packet_;

			internal:
				PacketWrapper(redav::media::Packet* packet);

				redav::media::Packet* GetPacket();
			};
		}
	}
}
