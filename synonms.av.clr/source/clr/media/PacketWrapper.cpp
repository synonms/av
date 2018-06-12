#include "PacketWrapper.h"

using namespace Synonms::Av::Clr::Media;

PacketWrapper::PacketWrapper(redav::media::Packet* packet)
	: _packet(packet)
{
}

redav::media::Packet* PacketWrapper::GetCorePacket()
{
	return _packet;
}
