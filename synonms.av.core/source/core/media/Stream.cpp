#include "Stream.h"

extern "C"
{
#include <libavformat/avformat.h>
}

#include <iostream>

using namespace synonms::av::encoding;
using namespace synonms::av::enumerators;
using namespace synonms::av::media;
using namespace synonms::av::utilities;

class Stream::Implementation
{
public:
	Implementation()
	{
	}

	AVStream* stream{ nullptr };

	void SetUp(const Format& format)
	{
		std::cout << "Stream: Creating stream..." << std::endl;

		stream = avformat_new_stream(format.GetAVFormatContext(), nullptr);

		if (stream == nullptr) throw std::exception("Could not allocate stream");

		stream->id = format.GetAVFormatContext()->nb_streams - 1;

		std::cout << "Stream: Stream id " << stream->id << " created" << std::endl;
	}

	void TearDown()
	{
	}
};

Stream::Stream()
{
	implementation = std::make_unique<Implementation>();
}

Stream::~Stream()
{
	implementation->TearDown();
}

void Stream::CopyParameters(const Codec& codec)
{
	if (avcodec_parameters_from_context(implementation->stream->codecpar, codec.GetCodecContext()) < 0) throw std::exception("Stream error: Could not copy the codec parameters");
}

AVStream* Stream::GetAVStream() const
{
	return implementation->stream;
}

MediaType Stream::GetMediaType() const
{
	return MediaTypeMapper::FromFfmpeg(implementation->stream->codecpar->codec_type);
}

CodecType Stream::GetAudioCodecType() const
{
	return CodecTypeMapper::FromFfmpeg(implementation->stream->codecpar->codec_id);
}

void Stream::Initialise(const Format& format)
{
	implementation->SetUp(format);
}

bool Stream::IsValid() const
{
	return implementation->stream != nullptr;
}

void Stream::SetAVStream(AVStream* stream)
{
	implementation->stream = stream;
}

void Stream::SetTimeBase(RationalNumber timeBase)
{
	implementation->stream->time_base = timeBase.GetAVRational();
}
