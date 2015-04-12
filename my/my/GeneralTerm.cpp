#include "StdAfx.h"

#include "GeneralTerm.h"

GeneralTerm::GeneralTerm(const coef_t& coefficients)
{
	this->coefficients = coefficients;
}

GeneralTerm::~GeneralTerm(){}

GeneralTerm* GeneralTerm::newInstance() const
{
	return new GeneralTerm(coefficients);
}

double GeneralTerm::funcVal(int n) const
{
	double result = 0;
	coef_t::const_iterator iter = coefficients.begin();
	int term = 1;
	while (iter != coefficients.end())
	{
		result += *iter * term;
		term *= n;
		iter++;
	}
	return round(result);
}

string GeneralTerm::toString() const
{
	string result = nameToString("n") + " = ";

	bool needPlusSign = false;
	int term = 1;
	coef_t::const_iterator iter = coefficients.begin();

	if (coefficients.empty() || coefficients.size() == 1 && *iter == 0)
	{
		return result + "0";
	}

	result += dtos(*iter, 0);
	if (*iter)
	{
		needPlusSign = true;
	}
	iter++;

	while (iter != coefficients.end())
	{
		if (*iter > 0)
		{
			if (term == 1)
			{
				if (needPlusSign)
				{
					result += " + " + dtos(*iter, 1) + "n";
				}
				else
				{
					result += dtos(*iter, 1) + "n";
				}
			}
			else
			{
				if (needPlusSign)
				{
					result += " + " + dtos(*iter, 1) + "n^" + dtos(term);
				}
				else
				{
					result += dtos(*iter, 1) + "n^" + dtos(term);
				}
			}
			needPlusSign = true;
		}

		if (*iter < 0)
		{
			if (term == 1)
			{
				if (needPlusSign)
				{
					result += " + (" + dtos(*iter) + ")n";
				}
				else
				{
					result += "(" + dtos(*iter) + ")n";
				}
			}
			else
			{
				if (needPlusSign)
				{
					result += " + (" + dtos(*iter) + ")n^" + dtos(term);
				}
				else
				{
					result += "(" + dtos(*iter) + ")n^" + dtos(term);
				}
			}
			needPlusSign = true;
		}

		term++;
		iter++;
	}
	return result;
}

string GeneralTerm::toString(const string &name)
{
	functionName = name;
	return toString();
}
