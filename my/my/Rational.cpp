#include "StdAfx.h"
#include "Rational.h"

Rational::Rational(void) // the default, value = 0
{
	mixed = false;
	numerator = 0;
	denominator = 1;
}

Rational::Rational(const Rational &r)
{
	this->denominator = r.denominator;
	this->numerator = r.numerator;
	this->mixed = r.mixed;
}

Rational::Rational(int val) // initialize with a integer value
{
	mixed = false;
	numerator = val;
	denominator = 1;
}

Rational::Rational(int numerator, int denominator) // initialize with two integer values
{
	mixed = false;
	this->numerator = numerator;
	this->denominator = denominator;
	reduction();
}

Rational::Rational(double val) // initialize with a double float
{
	mixed = false;
	denominator = 1;
	while(val != int(val))
	{
		val *= 10;
		denominator *= 10;
	}
	numerator = int(val);
	reduction();
}

void Rational::reduction(void)
{
	if(denominator == 0)
	{
		if(numerator > 0) // +INF
		{
			numerator = 1;
		}
		else if(numerator < 0) // -INF
		{
			numerator = -1;
		}
		// or 0/0 is NAN
		return;
	}
	if(numerator == 0) // the zero
	{
		denominator = 1;
		return;
	}
	if(denominator < 0)
	{
		numerator = -numerator;
		denominator = -denominator;
	}
	int _gcd = gcd(abs(numerator), denominator);
	numerator /= _gcd;
	denominator /= _gcd;
}

int Rational::gcd(int a, int b)
{
	if (b == 0)
	{
		return a;
	}
	return gcd(b, a % b);
}

void Rational::setMixed(bool mixed)
{
	this->mixed = mixed;
}

bool Rational::operator<(const Rational &r)const
{
	return numerator * r.denominator < r.numerator * denominator;
}

bool Rational::operator>(const Rational &r)const
{
	return numerator * r.denominator > r.numerator * denominator;
}

bool Rational::operator<=(const Rational &r)const
{
	return numerator * r.denominator <= r.numerator * denominator;
}

bool Rational::operator>=(const Rational &r)const
{
	return numerator * r.denominator >= r.numerator * denominator;
}

bool Rational::operator==(const Rational &r)const
{
	return numerator == r.numerator && denominator == r.denominator;
}

Rational Rational::operator+(const Rational &r)const
{
	Rational result;
	if(denominator == r.denominator)
	{
		result.denominator = denominator;
		result.numerator = numerator + r.numerator;
	}
	else
	{
		result.denominator = denominator * r.denominator;
		result.numerator = numerator * r.denominator + r.numerator * denominator;
	}
	result.reduction();
	return result;
}

Rational Rational::operator-()const
{
	Rational result;
	result.denominator = denominator;
	result.numerator = -numerator;
	return result;
}

Rational Rational::operator-(const Rational &r)const
{
	return *this + -r;
}

Rational Rational::operator*(const Rational &r)const
{
	Rational result;
	result.denominator = denominator * r.denominator;
	result.numerator = numerator * r.numerator;
	result.reduction();
	return result;
}

Rational Rational::operator/(const Rational &r)const
{
	Rational result;
	result.denominator = denominator * r.numerator;
	result.numerator = numerator * r.denominator;
	result.reduction();
	return result;
}

Rational Rational::reverse()const
{
	Rational result;
	result.denominator = abs(numerator);
	result.numerator = numerator >= 0 ? denominator : -denominator;
	result.reduction();
	return result;
}

Rational Rational::operator^(int n)const
{
	Rational result = 1;
	if(n == 0)
	{
		if(*this == 0)
		{
			return Rational(0,0);
		}
	}
	else if(n < 0)
	{
		n = -n;
		for(int i = 0; i < n; i++)
		{
			result = result * this->reverse();
		}
	}
	else
	{
		for(int i = 0; i < n; i++)
		{
			result = result * *this;
		}
	}
	return result;
}

int Rational::ceil(void)
{
	if (denominator == 0)
	{
		throw new VALUE_INVALID_FOR_CEIL;
	}
	if(denominator == 1) return numerator;
	if(numerator < 0)
	{
		return numerator / denominator;
	}
	return numerator / denominator + 1;
}

int Rational::floor(void)
{
	if (denominator == 0)
	{
		throw new VALUE_INVALID_FOR_FLOOR;
	}
	if(denominator == 1) return numerator;
	if(numerator < 0)
	{
		return numerator / denominator - 1;
	}
	return numerator / denominator;
}

ostream& operator<<(ostream &outStream, const Rational &r)
{
	if(r.denominator == 0)
	{
		if(r.numerator == 1)
		{
			outStream << "#+INF";
		}
		else if(r.numerator == -1)
		{
			outStream << "#-INF";
		}
		else
		{
			outStream << "#NAN";
		} 
	}
	else if(r.denominator == 1)
	{
		outStream << r.numerator;
	}
	else if (r.mixed && abs(r.numerator) > r.denominator)
	{
		outStream << r.numerator / r.denominator << "_" << abs(r.numerator) % r.denominator << "/" << r.denominator;
	}
	else
	{
		outStream << r.numerator << "/" << r.denominator;
	}
	return outStream;
}
