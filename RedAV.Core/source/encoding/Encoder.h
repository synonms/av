#pragma once

#include <functional>

#include <enumerators/CodecType.h>
#include <media/Frame.h>
#include <media/Packet.h>

struct AVCodec;
struct AVCodecContext;
struct AVDictionary;
struct AVFormatContext;
enum AVSampleFormat;
struct AVStream;
struct SwrContext;
enum AVPixelFormat;

namespace redav
{
	namespace encoding
	{
		class Encoder
		{
		public:
			explicit Encoder();
			~Encoder();

			void EncodeFrame(media::Frame* frame, const std::function<void(media::Packet*)>& packetCompleteDelegate);
			void Open(AVFormatContext* formatContext, AVStream* stream, enumerators::CodecType codecType, AVDictionary* options);

		private:
			AVStream* stream_{ nullptr };
			enumerators::CodecType codecType_{enumerators::CodecType::Unknown};
			AVCodec* codec_{ nullptr };
			AVCodecContext* codecContext_{ nullptr };
			AVFrame* encodedFrame_{ nullptr };
			AVFrame* tempFrame_{ nullptr };
			SwrContext* resampleContext_{ nullptr };
			int sampleCount_{ 0 };

			void UpdateAudioContext() const;
			void UpdateVideoContext() const;
			void OpenAudio(AVDictionary* options);
			void OpenVideo(AVDictionary* options);

			static AVFrame* CreateAudioFrame(AVSampleFormat sampleFormat, uint64_t channelLayout, int sampleRate, int noOfSamples);
			static AVFrame* CreateVideoFrame(AVPixelFormat pixelFormat, int width, int height);

			void PrepareAudioFrame(AVFrame* inputFrame);
			void PrepareVideoFrame(AVFrame* inputFrame);

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
