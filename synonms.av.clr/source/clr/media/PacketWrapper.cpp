#include "PacketWrapper.h"

using namespace Synonms::Av::Clr::Media;

PacketWrapper::PacketWrapper(synonms::av::media::Packet* packet)
	: _packet(packet)
{
}

synonms::av::media::Packet* PacketWrapper::GetCorePacket()
{
	return _packet;
}
