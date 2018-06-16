#pragma once

#include <core\utilities\Dictionary.h>

namespace Synonms
{
	namespace Av
	{
		namespace Clr
		{
			namespace Utilities
			{
				public ref class DictionaryWrapper
				{
				internal:
					DictionaryWrapper(synonms::av::utilities::Dictionary* dictionary);

					synonms::av::utilities::Dictionary* GetCoreDictionary();

				private:
					synonms::av::utilities::Dictionary* _dictionary;
				};
			}
		}
	}
}
