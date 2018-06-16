#include "RationalNumber.h"

using namespace Synonms::Av::Clr::Utilities;

RationalNumber::RationalNumber(const synonms::av::utilities::RationalNumber& rationalNumber)
	: _numerator(rationalNumber.GetNumerator())
	, _denominator(rationalNumber.GetDenominator())
{
}

RationalNumber::RationalNumber(int numerator, int denominator)
	: _numerator(numerator)
	, _denominator(denominator)
{
}

synonms::av::utilities::RationalNumber RationalNumber::GetCoreRationalNumber()
{
	return synonms::av::utilities::RationalNumber(_numerator, _denominator);
}
