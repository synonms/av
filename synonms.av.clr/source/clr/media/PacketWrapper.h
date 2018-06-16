#pragma once

#include <core\media\Packet.h>

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
					PacketWrapper(synonms::av::media::Packet* packet);

					synonms::av::media::Packet* GetCorePacket();

				private:
					synonms::av::media::Packet* _packet;
				};
			}
		}
	}
}
