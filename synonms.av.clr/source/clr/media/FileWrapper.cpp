#include "FileWrapper.h"

using namespace Synonms::Av::Clr::Media;

FileWrapper::FileWrapper(synonms::av::media::File* file)
	: _file(file)
{
}

synonms::av::media::File* FileWrapper::GetCoreFile()
{
	return _file;
}
