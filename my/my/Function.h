#pragma once

// Function.h

#include <vector>
#include "Operation.h"

class Function;

// [ f ] := f * g * h ...
class Transform
{
	friend class N_1Function;
	friend class N_2Function;
	friend class N_1_N_2Function;

public:
	Transform();
	void addBasicTrans(OP operation, const Function& function);
	double calcuate(double val, int n) const;
	double inverse(double val, int n) const;
	PRIORITY getPriority() const;
	string toString(const string &val, PRIORITY p, const string &n) const;

private:
	PRIORITY p;
	class BasicTrans
	{
		friend class Transform;
		friend class N_1Function;
		friend class N_2Function;
		friend class N_1_N_2Function;

	public:
		BasicTrans(OP op, const Function& function);
		BasicTrans(const BasicTrans &bt);
		~BasicTrans();
		double calculate(double val, int n) const;
		double inverse(double val, int n) const;
		string toString(const string &val, PRIORITY p, const string &n) const;

	private:
		Operation operation;
		Function* function;
	};

	typedef vector<BasicTrans> transVector_t;

	transVector_t transforms;
};

///////////////////////////////////////////////////////////////////////////

// abstract base class
class Function
{
public:
	Function();
	virtual ~Function();
	virtual Function* newInstance() const = 0;
	virtual double funcVal(int n) const = 0;
	virtual string nameToString(const string &n) const;
	virtual string toString() const = 0;
	virtual string toString(const string &name) = 0;

protected:
	string functionName;
private:
	static int number;
	static int getGlobal(); // number added one each time this is called
};

// f(n) := constVal
class constFunction : public Function
{
public:
	constFunction(double constVal);
	virtual ~constFunction();
	virtual constFunction* newInstance() const;
	virtual double funcVal(int n) const;
	virtual string toString() const;
	virtual string toString(const string &name);

private:
	double constVal;
};

// f(n) := [ f(n-1) ]
// f(input) := f(output)
class N_1Function : public Function
{
public:
	N_1Function(const Transform& trans_1, int input, double output);
	virtual ~N_1Function();
	virtual N_1Function* newInstance() const;
	virtual double funcVal(int n) const;
	virtual string toString() const;
	virtual string toString(const string &name);

private:
	int input;
	double output;
	Transform trans_1;
};

// f(n) := [ f(n-2) ]
// f(input) := f(output)
// f(input+1) := f(nextOutput)
class N_2Function : public Function
{
public:
	N_2Function(const Transform& trans_2, int input, double output, double nextOutput);
	virtual ~N_2Function();
	virtual N_2Function* newInstance() const;
	virtual double funcVal(int n) const;
	virtual string toString() const;
	virtual string toString(const string &name);

private:
	int input;
	double output;
	double nextOutput;
	Transform trans_2;
};

// f(n) := [ [ f(n-2) ] * [ f(n-1) ] ]
// f(input) := f(output)
// f(input+1) := f(nextOutput)
class N_1_N_2Function : public Function
{
public:
	N_1_N_2Function(const Transform& trans_1, const Transform& trans_2, const Transform& trans_other, int input, double output, double nextOutput, OP op);
	virtual ~N_1_N_2Function();
	virtual N_1_N_2Function* newInstance() const;
	virtual double funcVal(int n) const;
	virtual string toString() const;
	virtual string toString(const string &name);

private:
	int input;
	double output;
	double nextOutput;
	Transform trans_1;
	Transform trans_2;
	Transform trans_other;
	Operation operation;
};
