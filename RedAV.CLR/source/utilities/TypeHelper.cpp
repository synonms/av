#include "TypeHelper.h"

#include <msclr\marshal_cppstd.h>

using namespace System;

using namespace RedAV::CLR::Utilities;

System::String^ TypeHelper::ToManagedString(const std::string& value)
{
	return gcnew String(value.c_str());
}

std::string TypeHelper::ToNativeString(System::String^ value)
{
	return msclr::interop::marshal_as<std::string>(value);
}
