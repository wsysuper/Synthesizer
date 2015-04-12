#pragma once

class Rational
{
public:
	Rational(void);
	Rational(const Rational &r);
	Rational(int val);
	Rational(int numerator, int denominator);
	Rational(double val);

private:
	bool mixed;
	int numerator;
	int denominator;
	void reduction(void);
public:
	static int gcd(int a, int b);

	void setMixed(bool mixed);
	bool operator<(const Rational &r) const;
	bool operator>(const Rational &r) const;
	bool operator<=(const Rational &r) const;
	bool operator>=(const Rational &r) const;
	bool operator==(const Rational &r) const;
	Rational operator+(const Rational &r) const;
	Rational operator-() const;
	Rational operator-(const Rational &r) const;
	Rational operator*(const Rational &r) const;
	Rational reverse() const;
	Rational operator/(const Rational &r) const;
	Rational operator^(int n) const;
	int ceil(void);
	int floor(void);

	friend ostream& operator<<(ostream &outStream, const Rational &r);
};

class VALUE_INVALID_FOR_CEIL{};
class VALUE_INVALID_FOR_FLOOR{};
