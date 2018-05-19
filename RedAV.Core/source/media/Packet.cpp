#include "Packet.h"

extern "C"
{
#include <libavformat/avformat.h>
}

using namespace redav::media;

Packet::Packet(AVPacket* packet) 
	: packet_(packet) 
{}

AVPacket* Packet::GetPacket() const
{
	return packet_;
}
