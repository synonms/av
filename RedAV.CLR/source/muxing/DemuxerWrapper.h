#pragma once

#include <muxing\Demuxer.h>

#include <media\PacketWrapper.h>
#include <media\StreamWrapper.h>

namespace RedAV
{
	namespace CLR
	{
		namespace Muxing
		{
			public ref class DemuxerWrapper
			{
				redav::muxing::Demuxer* demuxer_;

				delegate void PacketReadDelegate(redav::media::Packet* packet);
				void OnPacketRead(redav::media::Packet* packet);

			public:
				DemuxerWrapper();
				~DemuxerWrapper();
				!DemuxerWrapper();

				event System::Action<Media::PacketWrapper^>^ PacketReadEvent;

				void Close();
				void ReadPackets();
				const System::Collections::Generic::List<Media::StreamWrapper^>^ GetStreams();
				void Open(System::String^ filePath);
			};
		}
	}
}
