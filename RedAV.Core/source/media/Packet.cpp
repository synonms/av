#include "Packet.h"

extern "C"
{
#include <libavformat/avformat.h>
}

using namespace redav::media;

Packet::Packet(AVPacket* packet) 
	: packet_(packet) 
{}

uint8_t* Packet::GetData() const
{
	return packet_->data;
}

uint64_t Packet::GetDuration() const
{
	return packet_->duration;
}

AVPacket* Packet::GetPacket() const
{
	return packet_;
}

int64_t Packet::GetPosition() const
{
	return packet_->pos;
}

int Packet::GetSize() const
{
	return packet_->size;
}

int Packet::GetStreamIndex() const
{
	return packet_->stream_index;
}
