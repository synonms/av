#pragma once

#include <memory>

struct AVRational;

namespace redav
{
	namespace utilities
	{
		class RationalNumber
		{
		public:
			RationalNumber(int numerator = 1, int denominator = 1);
			RationalNumber(const RationalNumber& other);
			RationalNumber& operator=(const RationalNumber& other);
			~RationalNumber();

			int GetDenominator() const;
			int GetNumerator() const;

		public: //Internal
			RationalNumber(AVRational rational);

			AVRational GetAVRational() const;

		private:
			class Implementation;
			std::unique_ptr<Implementation> implementation;
		};
	}
}