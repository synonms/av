#include "FormatWrapper.h"

using namespace Synonms::Av::Clr::Encoding;

FormatWrapper::FormatWrapper(synonms::av::encoding::Format* format)
	: _format(format)
{
}

synonms::av::encoding::Format* FormatWrapper::GetCoreFormat()
{
	return _format;
}
