#pragma once

#include <functional>

#include <enumerators/AudioCodec.h>
#include <media/Frame.h>
#include <media/Packet.h>

struct AVCodec;
struct AVCodecContext;
struct AVStream;

namespace redav
{
	namespace encoding
	{
		class Decoder
		{
		public:
			Decoder();
			~Decoder();

			void DecodePacket(media::Packet* packet, const std::function<void(media::Frame*)>& frameCompleteDelegate);
			void Open(AVStream* stream);
			void Open(enumerators::AudioCodec audioCodec);

		private:
			AVCodec* codec_{ nullptr };
			AVCodecContext* codecContext_{ nullptr };
			AVFrame* decodedFrame_{ nullptr };

			void SetCodecAndContext(AVCodecID codecID);

		};
	}
}
