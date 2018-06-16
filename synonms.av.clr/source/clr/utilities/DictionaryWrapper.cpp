#include "DictionaryWrapper.h"

using namespace Synonms::Av::Clr::Utilities;

DictionaryWrapper::DictionaryWrapper(synonms::av::utilities::Dictionary* dictionary)
	: _dictionary(dictionary)
{
}

synonms::av::utilities::Dictionary* DictionaryWrapper::GetCoreDictionary()
{
	return _dictionary;
}
