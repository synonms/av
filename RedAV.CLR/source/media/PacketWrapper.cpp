#include "PacketWrapper.h"

using namespace RedAV::CLR::Media;
using namespace redav::media;

PacketWrapper::PacketWrapper(Packet* packet)
	: packet_(packet)
{	
}

Packet* PacketWrapper::GetPacket()
{
	return packet_;
}
