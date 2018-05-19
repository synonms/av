#pragma once

#include <functional>

#include <enumerators/AudioCodec.h>
#include <media/Frame.h>
#include <media/Packet.h>

struct AVCodec;
struct AVCodecContext;
enum AVSampleFormat;

namespace redav
{
	namespace encoding
	{
		class Encoder
		{
		public:
			explicit Encoder(enumerators::AudioCodec audioCodec);
			~Encoder();

			void EncodeFrame(media::Frame* frame, const std::function<void(media::Packet*)>& packetCompleteDelegate);
			void Open();

		private:
			enumerators::AudioCodec audioCodec_;
			AVCodec* codec_{ nullptr };
			AVCodecContext* codecContext_{ nullptr };
			AVPacket* encodedPacket_{ nullptr };

			// TODO - Move to helper
			/* check that a given sample format is supported by the encoder */
			static bool IsSampleFormatSupported(AVCodec* codec, AVSampleFormat format);
			/* select layout with the highest channel count */
			static uint64_t SelectHighestChannelCount(const AVCodec* codec);
			/* just pick the highest supported samplerate */
			static int SelectHighestSampleRate(const AVCodec* codec);

		};
	}
}
