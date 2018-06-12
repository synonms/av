#pragma once

#include <media\Packet.h>

namespace Synonms
{
	namespace Av
	{
		namespace Clr
		{
			namespace Media
			{
				public ref class PacketWrapper
				{
				internal:
					PacketWrapper(redav::media::Packet* packet);

					redav::media::Packet* GetCorePacket();

				private:
					redav::media::Packet* _packet;
				};
			}
		}
	}
}
