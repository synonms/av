#pragma once

#include <functional>

#include <enumerators/AudioCodec.h>
#include <media/Frame.h>
#include <media/Packet.h>

struct AVCodec;
struct AVCodecContext;

namespace redav
{
	namespace encoding
	{
		class Decoder
		{
		public:
			explicit Decoder(enumerators::AudioCodec audioCodec);
			~Decoder();

			void DecodePacket(media::Packet* packet, const std::function<void(media::Frame*)>& frameCompleteDelegate);
			void Open();

		private:
			enumerators::AudioCodec audioCodec_;
			AVCodec* codec_{ nullptr };
			AVCodecContext* codecContext_{ nullptr };
			AVFrame* decodedFrame_{ nullptr };
		};
	}
}
