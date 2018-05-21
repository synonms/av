#pragma once

#include <vector>

#include <enumerators/CodecType.h>
#include <media/Frame.h>
#include <media/Stream.h>
#include <encoding/Encoder.h>

struct AVCodec;
struct AVCodecContext;
struct AVDictionary;
struct AVFormatContext;
struct AVFrame;
struct AVOutputFormat;
enum AVPixelFormat;
enum AVSampleFormat;
struct AVPacket;
struct AVRational;
struct SwrContext;

namespace redav
{
	namespace muxing
	{

		class Muxer
		{
			std::string filePath_;
			bool isInterleaved_{ false };
			std::vector<media::Stream> streams_;
			AVFormatContext* formatContext_{ nullptr };
			AVStream* audioStream_{ nullptr };
			AVStream* videoStream_{ nullptr };
			encoding::Encoder audioEncoder_;
			encoding::Encoder videoEncoder_;

			AVStream* CreateStream(AVCodecID codecId) const;

			int WritePacket(const utilities::RationalNumber& timeBase, AVStream* stream, AVPacket* packet) const;

		public:
			void Close();
			void Open(const std::string& filePath, enumerators::CodecType audioCodecType, bool isInterleaved = true);
			void WriteBytes(const std::vector<uint8_t>& data) const;
			void WriteFrame(media::Frame* frame);
		};
	}
}