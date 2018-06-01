#include "Format.h"

extern "C"
{
#include <libavformat/avformat.h>
}

#include <iostream>

using namespace redav::encoding;
using namespace redav::enumerators;
using namespace redav::media;

class Format::Implementation
{
public:
	Implementation(AVFormatContext* formatContext)
		: formatContext(formatContext)
	{
	}

	AVFormatContext* formatContext{ nullptr };

	void SetUp()
	{
		formatContext = avformat_alloc_context();

		if (formatContext == nullptr) throw std::exception("Format error: Failed to allocate context");
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

void Format::Initialise()
{
	implementation->SetUp();
}

bool Format::IsValid() const
{
	return implementation->formatContext != nullptr;
}

// TODO - This is output only - account for input
void Format::Open(const File& file)
{
	if (file.GetAVIOContext() == nullptr) throw std::exception("Format error: IO context is null");
	if (file.GetFilePath().empty()) throw std::exception("Format error: File path not set");

	/* Associate the output file with the container format context. */
	implementation->formatContext->pb = file.GetAVIOContext();

	std::cout << "Format: Guessing format..." << std::endl;

	implementation->formatContext->oformat = av_guess_format(nullptr, file.GetFilePath().c_str(), nullptr);

	if (implementation->formatContext->oformat == nullptr) throw std::exception("Format error: Failed to determine format from file path");

	std::cout << "Format: Format guessed - setting url..." << std::endl;

	implementation->formatContext->url = av_strdup(file.GetFilePath().c_str());

	std::cout << "Format: Open" << std::endl;
}
