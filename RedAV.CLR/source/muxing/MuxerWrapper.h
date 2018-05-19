#pragma once

#include <muxing\Muxer.h>

#include <enumerators\AudioCodecWrapper.h>
#include <media\PacketWrapper.h>

namespace RedAV
{
	namespace CLR
	{
		namespace Muxing
		{
			public ref class MuxerWrapper
			{
				redav::muxing::Muxer* muxer_;

			public:
				MuxerWrapper();
				~MuxerWrapper();
				!MuxerWrapper();

				void Close();
				void Open(System::String^ filePath, Enumerators::AudioCodecWrapper audioCodecType, bool isInterleaved);
				void WritePacket(Media::PacketWrapper^ packet);
			};
		}
	}
}
