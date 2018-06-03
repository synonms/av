#include "Packet.h"

extern "C"
{
#include <libavformat/avformat.h>
}

using namespace redav::media;

class Packet::Implementation
{
public:
	Implementation()
	{
		packet = av_packet_alloc();
	}

	AVPacket* packet;
};

Packet::Packet()
{
	implementation = std::make_unique<Implementation>();
}

Packet::~Packet()
{
	if (implementation->packet) av_packet_free(&implementation->packet);
}

AVPacket* Packet::GetAVPacket() const
{
	return implementation->packet;
}

uint8_t* Packet::GetData() const
{
	return implementation->packet->data;
}

uint64_t Packet::GetDuration() const
{
	return implementation->packet->duration;
}

int64_t Packet::GetPosition() const
{
	return implementation->packet->pos;
}

int Packet::GetSize() const
{
	return implementation->packet->size;
}

int Packet::GetStreamIndex() const
{
	return implementation->packet->stream_index;
}
