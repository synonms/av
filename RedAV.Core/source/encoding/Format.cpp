#include "Format.h"

extern "C"
{
#include <libavformat/avformat.h>
}

#include <iostream>

using namespace redav::encoding;
using namespace redav::enumerators;

class Format::Implementation
{
public:
	Implementation(AVFormatContext* formatContext)
		: formatContext(formatContext)
	{
	}

	AVFormatContext* formatContext{ nullptr };

	void SetUp(const std::string& filePath)
	{
		std::cout << "Format: Guessing format..." << std::endl;

		auto outputFormat = av_guess_format(nullptr, filePath.c_str(), nullptr);

		if (outputFormat == nullptr) throw std::exception("Format error: Failed to determine format from file path");

		std::cout << "Format: Format guessed, allocating context..." << std::endl;

		avformat_alloc_output_context2(&formatContext, outputFormat, nullptr, nullptr);

		if (formatContext == nullptr) throw std::exception("Format error: Failed to create format context");

		std::cout << "Format: Context created" << std::endl;
	}

	void TearDown()
	{
		if (formatContext != nullptr)
		{
			avformat_close_input(&formatContext);
			formatContext = nullptr;
		}
	}
};

Format::Format()
	: Format(nullptr)
{
}

Format::Format(AVFormatContext* formatContext)
{
	implementation = std::make_unique<Implementation>(formatContext);
}

Format::~Format()
{
	implementation->TearDown();
}

CodecType Format::GetAudioCodecType() const
{
	return CodecTypeMapper::FromFfmpeg(implementation->formatContext->audio_codec_id);
}

CodecType Format::GetVideoCodecType() const
{
	return CodecTypeMapper::FromFfmpeg(implementation->formatContext->video_codec_id);
}

AVFormatContext* Format::GetAVFormatContext() const
{
	return implementation->formatContext;
}

bool Format::HasGlobalHeader() const
{
	// #define AVFMT_GLOBALHEADER 0x0040
	return implementation->formatContext->oformat->flags & 0x0040;
}

void Format::Initialise(const std::string& filePath)
{
	implementation->SetUp(filePath);
}

bool Format::IsValid() const
{
	return implementation->formatContext != nullptr;
}
