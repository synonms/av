#pragma once

#include <utilities\RationalNumber.h>

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
					RationalNumber(const redav::utilities::RationalNumber& rationalNumber);

					redav::utilities::RationalNumber GetCoreRationalNumber();

				private:
					int _numerator;
					int _denominator;
				};
			}
		}
	}
}
