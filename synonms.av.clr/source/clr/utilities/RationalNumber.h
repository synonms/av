#pragma once

#include <core\utilities\RationalNumber.h>

namespace Synonms
{
	namespace Av
	{
		namespace Clr
		{
			namespace Utilities
			{
				public ref class RationalNumber
				{
				public:
					RationalNumber(int numerator, int denominator);

				internal:
					RationalNumber(const synonms::av::utilities::RationalNumber& rationalNumber);

					synonms::av::utilities::RationalNumber GetCoreRationalNumber();

				private:
					int _numerator;
					int _denominator;
				};
			}
		}
	}
}
