#include "File.h"

extern "C"
{
#include <libavformat/avformat.h>
}

using namespace synonms::av::media;

class File::Implementation
{
public:
	Implementation()
	{				
	}

	AVIOContext* ioContext{nullptr};
	std::string filePath{""};
};

File::File()
{
	implementation = std::make_unique<Implementation>();
}

File::~File()
{
	Close();
}

void File::Close()
{
	if (implementation->ioContext != nullptr)
	{
		avio_closep(&implementation->ioContext);
		implementation->ioContext = nullptr;
	}
}

const std::string& File::GetFilePath() const
{
	return implementation->filePath;
}

void File::Open(const std::string& filePath)
{
#define AVIO_FLAG_WRITE 2

	implementation->filePath = filePath;

	if (avio_open(&implementation->ioContext, filePath.c_str(), AVIO_FLAG_WRITE) < 0) throw std::exception(("File: Failed to open " + filePath).c_str());
}

AVIOContext* File::GetAVIOContext() const
{
	return implementation->ioContext;
}
