#include "Dictionary.h"

extern "C"
{
#include <libavutil/dict.h>
}

using namespace synonms::av::utilities;

class Dictionary::Implementation
{
public:
	Implementation(AVDictionary* dictionary)
		: dictionary(dictionary)
	{
	}

	AVDictionary* dictionary{ nullptr };
};

Dictionary::Dictionary()
	: Dictionary(nullptr)
{
}

Dictionary::Dictionary(AVDictionary* dictionary)
{
	implementation = std::make_unique<Implementation>(dictionary);
}

Dictionary::Dictionary(const Dictionary& dictionary)
{
	AVDictionary* dictionaryCopy{ nullptr };

	av_dict_copy(&dictionaryCopy, dictionary.GetAVDictionary(), 0);

	implementation = std::make_unique<Implementation>(dictionaryCopy);
}

Dictionary::~Dictionary()
{
	av_dict_free(&implementation->dictionary);
}

Dictionary& Dictionary::operator=(const Dictionary& dictionary)
{
	if (implementation->dictionary != nullptr) av_dict_free(&implementation->dictionary);

	av_dict_copy(&implementation->dictionary, dictionary.GetAVDictionary(), 0);

	return *this;
}

AVDictionary * Dictionary::GetAVDictionary() const
{
	return implementation->dictionary;
}
