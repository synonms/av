#pragma once

#include <string>

namespace Synonms
{
	namespace Av
	{
		namespace Clr
		{
			namespace Utilities
			{
				public ref class TypeHelper
				{
				public:
					static System::String^ ToManagedString(const std::string& value);
					static std::string ToNativeString(System::String^ value);
				};
			}
		}
	}
}