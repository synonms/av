#pragma once

namespace redav
{
	namespace utilities
	{
		class RationalNumber
		{
		public:
			RationalNumber(int num, int den)
				: numerator(num), denominator(den)
			{}

			int numerator;
			int denominator;
		};
	}
}