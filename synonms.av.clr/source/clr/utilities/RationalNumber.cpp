#include "RationalNumber.h"

using namespace Synonms::Av::Clr::Utilities;

RationalNumber::RationalNumber(const redav::utilities::RationalNumber& rationalNumber)
	: _numerator(rationalNumber.GetNumerator())
	, _denominator(rationalNumber.GetDenominator())
{
}

RationalNumber::RationalNumber(int numerator, int denominator)
	: _numerator(numerator)
	, _denominator(denominator)
{
}

redav::utilities::RationalNumber RationalNumber::GetCoreRationalNumber()
{
	return redav::utilities::RationalNumber(_numerator, _denominator);
}
