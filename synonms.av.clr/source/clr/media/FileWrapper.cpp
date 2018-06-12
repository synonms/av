#include "FileWrapper.h"

using namespace Synonms::Av::Clr::Media;

FileWrapper::FileWrapper(redav::media::File* file)
	: _file(file)
{
}

redav::media::File* FileWrapper::GetCoreFile()
{
	return _file;
}
