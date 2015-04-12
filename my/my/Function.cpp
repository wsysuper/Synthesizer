#include "StdAfx.h"

#include "Function.h"

Transform::BasicTrans::BasicTrans(OP op, const Function& function)
{
	this->operation = op;
	this->function = function.newInstance();
}

Transform::BasicTrans::BasicTrans(const BasicTrans &bt)
{
	this->operation = bt.operation;
	this->function = bt.function->newInstance();
}

Transform::BasicTrans::~BasicTrans()
{
	if (function)
	{
		delete function;
		function = NULL;
	}
}

double Transform::BasicTrans::calculate(double val, int n) const
{
	if (!function) throw 999;
	return operation.calculate(val, function->funcVal(n));
}

double Transform::BasicTrans::inverse(double val, int n) const
{
	if (!function) throw 999;
	return operation.inverse1(val, function->funcVal(n));
}

string Transform::BasicTrans::toString(const string &val, PRIORITY p, const string &n) const
{
	return operation.toString(val, p, function->nameToString(n), SINGLE);
}

//////////////////////////////////////////////////////////////////////////

Transform::Transform()
{
	p = SINGLE;
}

void Transform::addBasicTrans(OP op, const Function& function)
{
	BasicTrans bt(op, function);
	transforms.push_back(bt);
	p = bt.operation.getPriority();
}

double Transform::calcuate(double val, int n) const
{
	transVector_t::const_iterator iter;
	for(iter = transforms.begin(); iter != transforms.end(); iter++)
	{
		val = iter->calculate(val, n);
	}
	return val;
}

double Transform::inverse(double val, int n) const
{
	transVector_t::const_iterator iter = transforms.end();
	while (iter != transforms.begin())
	{
		iter--;
		val = iter->inverse(val, n);
	}
	return val;
}

PRIORITY Transform::getPriority() const
{
	return p;
}

string Transform::toString(const string &val, PRIORITY p, const string &n) const
{
	string result = val;
	transVector_t::const_iterator iter;
	for(iter = transforms.begin(); iter != transforms.end(); iter++)
	{
		result = iter->toString(result, p, n);
		p = iter->operation.getPriority();
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////

int Function::number = 0;

Function::Function()
{
	char buf[20];
	int global = getGlobal();
	_itoa_s(global, buf, 20, 10);
	functionName = "temp";
	functionName += buf;
}

Function::~Function(){}

string Function::nameToString(const string &n) const
{
	return functionName + "(" + n + ")";
}

int Function::getGlobal()
{
	return number++;
}

///////////////////////////////////////////////////////////////////////////

constFunction::constFunction(double constVal)
{
	this->constVal = constVal;
}

constFunction::~constFunction(){}

constFunction* constFunction::newInstance() const
{
	return new constFunction(constVal);
}

double constFunction::funcVal(int n) const
{
	return constVal;
}

string constFunction::toString() const
{
	string result = nameToString("n") + " = ";
	result += dtos(constVal);

	return result;
}

string constFunction::toString(const string &name)
{
	functionName = name;
	return toString();
}

///////////////////////////////////////////////////////////////////////////

N_1Function::N_1Function(const Transform& trans_1, int input, double output)
{
	this->input = input;
	this->output = output;
	this->trans_1 = trans_1;
}

N_1Function::~N_1Function(){}

N_1Function* N_1Function::newInstance() const
{
	return new N_1Function(trans_1, input, output);
}

double N_1Function::funcVal(int n) const
{
	if (n < input)
	{
		return trans_1.inverse(funcVal(n + 1), n + 1);
	}
	if (n == input)
	{
		return output;
	}
	return trans_1.calcuate(funcVal(n - 1), n);
}

string N_1Function::toString() const
{
	string result = nameToString("n") + " = ";
	result += trans_1.toString(nameToString("n-1"), SINGLE, "n");
	result += ", " + nameToString(dtos(input)) + " = ";

	result += dtos(output);

	Transform::transVector_t::const_iterator iter = trans_1.transforms.begin();
	for(; iter != trans_1.transforms.end(); iter++)
	{
		result += "\n" + iter->function->toString();
	}

	return result;
}

string N_1Function::toString(const string &name)
{
	functionName = name;
	return toString();
}

///////////////////////////////////////////////////////////////////////////

N_2Function::N_2Function(const Transform& trans_2, int input, double output, double nextOutput)
{
	this->input = input;
	this->output = output;
	this->nextOutput = nextOutput;
	this->trans_2 = trans_2;
}

N_2Function::~N_2Function(){}

N_2Function* N_2Function::newInstance() const
{
	return new N_2Function(trans_2, input, output, nextOutput);
}

double N_2Function::funcVal(int n) const
{
	if (n < input)
	{
		return trans_2.inverse(funcVal(n + 2), n + 2);
	}
	if (n == input)
	{
		return output;
	}
	if (n == input + 1)
	{
		return nextOutput;
	}
	return trans_2.calcuate(funcVal(n - 2), n);
}

string N_2Function::toString() const
{
	string result = nameToString("n") + " = ";
	result += trans_2.toString(nameToString("n-2"), SINGLE, "n");
	result += ", " + nameToString(dtos(input)) + " = ";

	result += dtos(output);

	result += ", " + nameToString(dtos(input + 1)) + " = ";

	result += dtos(nextOutput);

	Transform::transVector_t::const_iterator iter = trans_2.transforms.begin();
	for(; iter != trans_2.transforms.end(); iter++)
	{
		result += "\n" + iter->function->toString();
	}

	return result;
}

string N_2Function::toString(const string &name)
{
	functionName = name;
	return toString();
}

///////////////////////////////////////////////////////////////////////////

N_1_N_2Function::N_1_N_2Function(const Transform& trans_1, const Transform& trans_2, const Transform& trans_other, int input, double output, double nextOutput, OP op) // has_1 && has_2
{
	this->input = input;
	this->output = output;
	this->nextOutput = nextOutput;

	this->trans_1 = trans_1;
	this->trans_2 = trans_2;
	this->trans_other = trans_other;
	operation = op;
}

N_1_N_2Function::~N_1_N_2Function(){}

N_1_N_2Function* N_1_N_2Function::newInstance() const
{
	return new N_1_N_2Function(trans_1, trans_2, trans_other, input, output, nextOutput, operation.getOp());
}

double N_1_N_2Function::funcVal(int n) const
{
	if (n < input)
	{
		// f(n+2) = [ [ f(n) ] * [ f(n+1) ] ]
		double a = trans_other.inverse(funcVal(n + 2), n + 2); // [ f(n) ] * [ f(n+1) ]
		double b = operation.inverse1(a, trans_1.calcuate(funcVal(n + 1), n + 2)); // [ f(n) ]
		return trans_2.inverse(b, n + 2); // f(n)
	}
	if (n == input)
	{
		return output;
	}
	if (n == input + 1)
	{
		return nextOutput;
	}
	double n_2 = trans_2.calcuate(funcVal(n - 2), n);
	double n_1 = trans_1.calcuate(funcVal(n - 1), n);
	return trans_other.calcuate(operation.calculate(n_2, n_1), n);
}

string N_1_N_2Function::toString() const
{
	string result = nameToString("n") + " = ";
	string n_1 = trans_1.toString(nameToString("n-1"), SINGLE, "n");
	string n_2 = trans_2.toString(nameToString("n-2"), SINGLE, "n");
	PRIORITY p1 = trans_1.getPriority() ? trans_1.getPriority() : SINGLE;
	PRIORITY p2 = trans_2.getPriority() ? trans_2.getPriority() : SINGLE;
	result += trans_other.toString(operation.toString(n_2, p2, n_1, p1), operation.getPriority(), "n");

	result += ", " + nameToString(dtos(input)) + " = ";

	result += dtos(output);

	result += ", " + nameToString(dtos(input + 1)) + " = ";

	result += dtos(nextOutput);

	Transform::transVector_t::const_iterator iter;

	iter = trans_2.transforms.begin();
	for(; iter != trans_2.transforms.end(); iter++)
	{
		result += "\n" + iter->function->toString();
	}

	iter = trans_1.transforms.begin();
	for(; iter != trans_1.transforms.end(); iter++)
	{
		result += "\n" + iter->function->toString();
	}

	iter = trans_other.transforms.begin();
	for(; iter != trans_other.transforms.end(); iter++)
	{
		result += "\n" + iter->function->toString();
	}

	return result;
}

string N_1_N_2Function::toString(const string &name)
{
	functionName = name;
	return toString();
}

///////////////////////////////////////////////////////////////////////////
