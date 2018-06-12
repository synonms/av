#pragma once

#include <utilities\Dictionary.h>

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
					DictionaryWrapper(redav::utilities::Dictionary* dictionary);

					redav::utilities::Dictionary* GetCoreDictionary();

				private:
					redav::utilities::Dictionary* _dictionary;
				};
			}
		}
	}
}
