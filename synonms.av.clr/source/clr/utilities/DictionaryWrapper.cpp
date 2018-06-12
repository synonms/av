#include "DictionaryWrapper.h"

using namespace Synonms::Av::Clr::Utilities;

DictionaryWrapper::DictionaryWrapper(redav::utilities::Dictionary* dictionary)
	: _dictionary(dictionary)
{
}

redav::utilities::Dictionary* DictionaryWrapper::GetCoreDictionary()
{
	return _dictionary;
}
