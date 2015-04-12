#pragma once
// Operation.h

enum OP
{
	NONE,
	PLUS,
	MINUS,
	R_MINUS,
	MUL,
	DIV,
	R_DIV,
};

enum PRIORITY
{
	PLUS_MUNUS = 1,
	MUL_DIV = 2,
	SINGLE = 3,
};

class Operation
{
public:
	Operation();
	Operation(OP op);
	// TODO: this left public??
	double calculate(double num1, double num2) const;
	double inverse1(double result, double num2) const; // known result and num2, return num1
	double inverse2(double result, double num1) const; // known result and num1, return num2
	PRIORITY getPriority() const;
	OP getOp() const;
	string toString(const string &num1, PRIORITY p1, const string &num2, PRIORITY p2) const;

private:
	PRIORITY p;
	OP op;
};
