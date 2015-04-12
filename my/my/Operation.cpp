#include "StdAfx.h"

#include "Operation.h"

Operation::Operation()
{
	op = NONE;
	p = SINGLE;
}

Operation::Operation(OP op)
{
	this->op = op;
	p = SINGLE;
	switch (op)
	{
	case PLUS:
	case MINUS:
	case R_MINUS:
		p = PLUS_MUNUS;
		break;
	case MUL:
	case DIV:
	case R_DIV:
		p =MUL_DIV;
		break;
	}
}

double Operation::calculate(double num1, double num2) const
{
	double result = 0;
	switch (op)
	{
	case PLUS:
		result = num1 + num2;
		break;
	case MINUS:
		result = num1 - num2;
		break;
	case R_MINUS:
		result = num2 - num1;
		break;
	case MUL:
		result = num1 * num2;
		break;
	case DIV:
		result = num1 / num2;
		break;
	case R_DIV:
		result = num2 / num1;
		break;
	}
	return result;
}

double Operation::inverse1(double result, double num2) const
{
	double num1 = 0;
	switch (op)
	{
	case PLUS:
		num1 = result - num2;
		break;
	case MINUS:
		num1 = result + num2;
		break;
	case R_MINUS:
		num1 = num2 - result;
		break;
	case MUL:
		num1 = result / num2;
		break;
	case DIV:
		num1 = result * num2;
		break;
	case R_DIV:
		num1 = num2 / result;
		break;
	}
	return num1;
}

double Operation::inverse2(double result, double num1) const
{
	double num2 = 0;
	switch (op)
	{
	case PLUS:
		num2 = result - num1;
		break;
	case MINUS:
		num2 = num1 - result;
		break;
	case R_MINUS:
		num2 = num1 + result;
		break;
	case MUL:
		num2 = result / num1;
		break;
	case DIV:
		num2 = num1 / result;
		break;
	case R_DIV:
		num2 = num1 * result;
		break;
	}
	return num2;
}

OP Operation::getOp() const
{
	return op;
}

PRIORITY Operation::getPriority() const
{
	return p;
}

string Operation::toString(const string &num1, PRIORITY p1, const string &num2, PRIORITY p2) const
{
	string result, left, right;
	switch (op)
	{
	case PLUS:
		left = num1;
		right = num2;
		if (p1 < PLUS_MUNUS)
		{
			left = "(" + left + ")";
		}
		if (p2 <= PLUS_MUNUS)
		{
			right = "(" + right + ")";
		}
		result = left + " + " + right;
		break;
	case MINUS:
		left = num1;
		right = num2;
		if (p1 < PLUS_MUNUS)
		{
			left = "(" + left + ")";
		}
		if (p2 <= PLUS_MUNUS)
		{
			right = "(" + right + ")";
		}
		result = left + " - " + right;
		break;
	case R_MINUS:
		left = num2;
		right = num1;
		if (p2 < PLUS_MUNUS)
		{
			left = "(" + left + ")";
		}
		if (p1 <= PLUS_MUNUS)
		{
			right = "(" + right + ")";
		}
		result = left + " - " + right;
		break;
	case MUL:
		left = num1;
		right = num2;
		if (p1 < MUL_DIV)
		{
			left = "(" + left + ")";
		}
		if (p2 <= MUL_DIV)
		{
			right = "(" + right + ")";
		}
		result = left + " * " + right;
		break;
	case DIV:
		left = num1;
		right = num2;
		if (p1 < MUL_DIV)
		{
			left = "(" + left + ")";
		}
		if (p2 <= MUL_DIV)
		{
			right = "(" + right + ")";
		}
		result = left + " / " + right;
		break;
	case R_DIV:
		left = num2;
		right = num1;
		if (p2 < MUL_DIV)
		{
			left = "(" + left + ")";
		}
		if (p1 <= MUL_DIV)
		{
			right = "(" + right + ")";
		}
		result = left + " / " + right;
		break;
	}
	return result;
}
