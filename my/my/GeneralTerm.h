#pragma once

// GeneralTerm.h

#include "Function.h"

typedef vector<double> coef_t;

class GeneralTerm : public Function
{
public:
	GeneralTerm(const coef_t& coefficients);
	virtual ~GeneralTerm();
	virtual GeneralTerm* newInstance() const;
	virtual double funcVal(int n) const;
	virtual string toString() const;
	virtual string toString(const string &name);

private:
	coef_t coefficients; // (a, b, c, d, ...) in a + bn + cn^2 + dn^3 + ...
};
