#include "RationalNumber.h"

extern "C"
{
#include <libavutil/avutil.h>
}

using namespace synonms::av::utilities;

class RationalNumber::Implementation
{
public:
	Implementation(int numerator, int denominator)
		: numerator(numerator)
		, denominator(denominator)
	{
	}

	int numerator;
	int denominator;
};

RationalNumber::RationalNumber(int numerator, int denominator)
{
	implementation = std::make_unique<Implementation>(numerator, denominator);
}

RationalNumber::RationalNumber(const RationalNumber& other)
	: RationalNumber(other.GetNumerator(), other.GetDenominator())
{
}

RationalNumber::RationalNumber(AVRational rational)
	:RationalNumber(rational.num, rational.den)
{
}

RationalNumber::~RationalNumber()
{
}

RationalNumber& RationalNumber::operator=(const RationalNumber& other)
{
	implementation->numerator = other.GetNumerator();
	implementation->denominator = other.GetDenominator();

	return *this;
}

int RationalNumber::GetDenominator() const
{
	return implementation->denominator;
}

int RationalNumber::GetNumerator() const
{
	return implementation->numerator;
}

AVRational RationalNumber::GetAVRational() const
{
	return { implementation->numerator, implementation->denominator };
}
