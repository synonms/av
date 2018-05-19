#pragma once

#include <string>

namespace RedAV
{
	namespace CLR
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