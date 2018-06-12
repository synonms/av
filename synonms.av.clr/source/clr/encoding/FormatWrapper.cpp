#include "FormatWrapper.h"

using namespace Synonms::Av::Clr::Encoding;
using namespace redav::encoding;

FormatWrapper::FormatWrapper(redav::encoding::Format* format)
	: _format(format)
{
}

redav::encoding::Format* FormatWrapper::GetCoreFormat()
{
	return _format;
}
