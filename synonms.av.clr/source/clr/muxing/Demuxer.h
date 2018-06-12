#pragma once

#include <muxing\Demuxer.h>

#include <clr\encoding\DecoderWrapper.h>
#include <clr\enumerators\MediaType.h>
#include <clr\media\FrameWrapper.h>

namespace Synonms
{
	namespace Av
	{
		namespace Clr
		{
			namespace Muxing
			{
				public ref class Demuxer
				{
				public:
					Demuxer();
					~Demuxer();
					!Demuxer();

					event System::Action<Media::FrameWrapper^>^ FrameDecodedEvent;

					void Close();
					void DecodePackets(Enumerators::MediaType mediaType);
					Encoding::DecoderWrapper^ GetAudioDecoder();
					Encoding::DecoderWrapper^ GetVideoDecoder();
					void Open(System::String^ filePath);

				private:
					redav::muxing::Demuxer* _demuxer;

					delegate void FrameDecodedDelegate(const redav::media::Frame& frame);
					void OnFrameDecoded(const redav::media::Frame& frame);
				};
			}
		}
	}
}
