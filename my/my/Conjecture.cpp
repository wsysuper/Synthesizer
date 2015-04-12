#include "StdAfx.h"

#include "Conjecture.h"
#include "SMatrix.h"

// the minimal iopair group number to start
#define MIN_GROUP	3
#define MAX_GROUP	10
#define ACCEPTENCE	3

Conjecture::Conjecture()
{
	OPPri.push_back(PLUS);
	OPPri.push_back(MUL);
	OPPri.push_back(R_MINUS);

	const Transform nothing;
	Transform t;
	TransPri.push_back(nothing);

	t = nothing;
	t.addBasicTrans(PLUS, constFunction(1));
	TransPri.push_back(t);

	t = nothing;
	t.addBasicTrans(PLUS, constFunction(2));
	TransPri.push_back(t);

	t = nothing;
	t.addBasicTrans(MINUS, constFunction(1));
	TransPri.push_back(t);

	t = nothing;
	t.addBasicTrans(MINUS, constFunction(2));
	TransPri.push_back(t);

	t = nothing;
	t.addBasicTrans(MUL, constFunction(2));
	TransPri.push_back(t);

	t = nothing;
	t.addBasicTrans(MUL, constFunction(3));
	TransPri.push_back(t);

	t = nothing;
	t.addBasicTrans(DIV, constFunction(2));
	TransPri.push_back(t);
}

Function* Conjecture::generalTerm(const ioPair_t &ioPair, int &cd)
{
	if (ioPair.empty())
	{
		cd = 0;
		return NULL;
	}
	ioPair_t::const_iterator iter = ioPair.begin();

	int size = ioPair.size();
	SMatrix X(size, size);
	SMatrix Y(1, size);

	for (int i = 0; i < size; i++)
	{
		X[0][i] = 1;
		for (int j = 1; j < size; j++)
		{
			X[j][i] = X[j - 1][i] * iter->first;
		}
		Y[0][i] = iter->second;
		iter++;
	}

	SMatrix A = Y * X.Invert();

	coef_t coefficients;
	for (int i = 0; i < size; i++)
	{
		coefficients.push_back(round(A[0][i]));
	}

	cd = 1;
	// remove the useless zeros
	while(!coefficients.empty() && *--coefficients.end() == 0)
	{
		coefficients.pop_back();
		cd++;
	}

	return new GeneralTerm(coefficients);
}

// pre: ioPair.size() >= 1
bool Conjecture::allSame(const ioPair_t &ioPair)
{
	ioPair_t::const_iterator iter = ioPair.begin();
	double begin = iter->second;
	if (begin == 1.0 / zero || begin == -1.0 / zero || begin == 0.0 / zero)
	{
		return false;
	}
	iter++;
	while (iter != ioPair.end())
	{
		if (iter->second != begin)
		{
			return false;
		}
		iter++;
	}
	return true;
}

ioPair_t Conjecture::diffMapN_1(const ioPair_t &ioPair, const Operation &op)
{
	ioPair_t diff_ioPair;
	ioPair_t::const_iterator iter = ioPair.begin();
	int first = iter->first;
	double second = iter->second;
	iter++;

	while (iter != ioPair.end())
	{
		if (iter->first == first + 1)
		{
			diff_ioPair[iter->first] = op.inverse2(iter->second, second);
		}
		first = iter->first;
		second = iter->second;
		iter++;
	}

	return diff_ioPair;
}

ioPair_t Conjecture::diffMapN_2(const ioPair_t &ioPair, const Operation &op)
{
	ioPair_t diff_ioPair;
	ioPair_t::const_iterator iter = ioPair.begin();
	int first_1 = iter->first;
	double second_1 = iter->second;
	iter++;
	int first_2 = iter->first;
	double second_2 = iter->second;
	iter++;

	while (iter != ioPair.end())
	{
		if (iter->first == first_1 + 2)
		{
			diff_ioPair[iter->first] = op.inverse2(iter->second, second_1);
		}
		first_1 = first_2;
		second_1 = second_2;
		first_2 = iter->first;
		second_2 = iter->second;
		iter++;
	}

	return diff_ioPair;
}

ioPair_t Conjecture::diffMapN_1_N_2(const ioPair_t &ioPair, const Transform &trans_1, const Transform &trans_2, const Operation &op, const Operation &newOp)
{
	ioPair_t diff_ioPair;
	ioPair_t::const_iterator iter = ioPair.begin();
	int first_2 = iter->first;
	double second_2 = iter->second;
	iter++;
	int first_1 = iter->first;
	double second_1 = iter->second;
	iter++;

	while (iter != ioPair.end())
	{
		if (iter->first == first_2 + 2)
		{
			diff_ioPair[iter->first] = newOp.inverse2(iter->second, op.calculate(trans_2.calcuate(second_2, iter->first), trans_1.calcuate(second_1, iter->first)));
		}
		first_2 = first_1;
		second_2 = second_1;
		first_1 = iter->first;
		second_1 = iter->second;
		iter++;
	}

	return diff_ioPair;
}

// pre: group_num < ioPair.size()
ioPair_t Conjecture::extract(const ioPair_t &ioPair, unsigned &group_num)
{
	ioPair_t newIOPair;

	ioPair_t::const_iterator iter = ioPair.begin();
	for (unsigned i = 0; i < group_num; i++)
	{
		newIOPair[iter->first] = iter->second;
		iter++;
	}
	return newIOPair;
}

bool Conjecture::check(const ioPair_t &ioPair, Function* f)
{
	if (f == NULL)
	{
		return false;
	}
	ioPair_t::const_iterator iter;
	for (iter = ioPair.begin(); iter != ioPair.end(); iter++)
	{
		if (f->funcVal(iter->first) != iter->second)
		{
			return false;
		}
	}
	return true;
}

// pre: !ioPair.empty() && !allSame(ioPair)
Function* Conjecture::synthesiseN_1(const ioPair_t &ioPair, const Operation &op, int &cd)
{
	int input = ioPair.begin()->first;
	double output = ioPair.begin()->second;

	ioPair_t diff_ioPair = diffMapN_1(ioPair, op);

	Function* diff_func = nonspeedup_synthesise(false, diff_ioPair, cd);

	if (diff_func == NULL)
	{
		return NULL;
	}

	Transform trans;
	trans.addBasicTrans(op.getOp(), *diff_func);
	delete diff_func;
	return new N_1Function(trans, input, output);
}

Function* Conjecture::synthesiseN_2(const ioPair_t &ioPair, const Operation &op, int &cd)
{
	int input;
	double output, nextOutput;
	bool success = false;
	ioPair_t::const_iterator iter = ioPair.begin();
	while(iter != ioPair.end())
	{
		if (ioPair.count(iter->first + 1))
		{
			input = iter->first;
			output = iter->second;
			nextOutput = ioPair.at(iter->first + 1);
			success = true;
			break;
		}
		iter++;
	}
	if (!success)
	{
		cd = 0;
		return NULL;
	}

	ioPair_t diff_ioPair = diffMapN_2(ioPair, op);

	Function* diff_func = nonspeedup_synthesise(false, diff_ioPair, cd);

	if (diff_func == NULL)
	{
		return NULL;
	}

	Transform trans;
	trans.addBasicTrans(op.getOp(), *diff_func);
	delete diff_func;
	return new N_2Function(trans, input, output, nextOutput);
}

Function* Conjecture::synthesiseN_1_N_2(const ioPair_t &ioPair, Transform trans_1, Transform trans_2, const Operation &op, const Operation &newOp, int &cd)
{
	Function *result = NULL;
	cd = 0;
	Transform trans_other;
	int input;
	double output, nextOutput;
	bool success = false;
	ioPair_t::const_iterator iter = ioPair.begin();
	while(iter != ioPair.end())
	{
		if (ioPair.count(iter->first + 1))
		{
			input = iter->first;
			output = iter->second;
			nextOutput = ioPair.at(iter->first + 1);
			success = true;
			break;
		}
		iter++;
	}
	if (!success)
	{
		return NULL;
	}

	if (newOp.getOp() == NONE)
	{
		result = new N_1_N_2Function(trans_1, trans_2, trans_other, input, output, nextOutput, op.getOp());
		if (check(ioPair, result))
		{
			cd = ioPair.size() - 2;
			return result;
		}
		else
		{
			delete result;
			return NULL;
		}
	}
	else
	{
		ioPair_t diff_ioPair = diffMapN_1_N_2(ioPair, trans_1, trans_2, op, newOp);

		Function* diff_func = nonspeedup_synthesise(false, diff_ioPair, cd);

		if (diff_func == NULL)
		{
			return NULL;
		}

		trans_other.addBasicTrans(newOp.getOp(), *diff_func);
		delete diff_func;
		return new N_1_N_2Function(trans_1, trans_2, trans_other, input, output, nextOutput, op.getOp());
	}
}

Function* Conjecture::processN_1(const ioPair_t &ioPair, int &cd)
{
	Function* f = NULL;
	Function* result = NULL;
	int confidenceDegree;

	for (OPVector_t::const_iterator iter = OPPri.begin(); iter != OPPri.end(); iter++)
	{
		if (f && result != f)
		{
			delete f;
		}
		f = synthesiseN_1(ioPair, *iter, confidenceDegree);
		if (confidenceDegree > cd)
		{
			if (result)
			{
				delete result;
			}
			cd = confidenceDegree;
			result = f;
		}
	}
	return result;
}

Function* Conjecture::processN_2(const ioPair_t &ioPair, int &cd)
{
	Function* f = NULL;
	Function* result = NULL;
	int confidenceDegree;

	for (OPVector_t::const_iterator iter = OPPri.begin(); iter != OPPri.end(); iter++)
	{
		if (f && result != f)
		{
			delete f;
		}
		f = synthesiseN_2(ioPair, *iter, confidenceDegree);
		if (confidenceDegree > cd)
		{
			if (result)
			{
				delete result;
			}
			cd = confidenceDegree;
			result = f;
		}
	}
	return result;
}

Function* Conjecture::processN_1_N_2(const ioPair_t &ioPair, int &cd)
{
	Function* f = NULL;
	Function* result = NULL;
	int confidenceDegree;

	for (int op = PLUS; op <= R_DIV; op++)
	{
		for (TransformVector_t::const_iterator iter1 = TransPri.begin(); iter1 != TransPri.end(); iter1++)
		{
			if (Operation((OP)op).getPriority() == iter1->getPriority())
			{
				continue;
			}
			for (TransformVector_t::const_iterator iter2 = TransPri.begin(); iter2 != TransPri.end(); iter2++)
			{
				if (Operation((OP)op).getPriority() == iter2->getPriority())
				{
					continue;
				}
				if (f && result != f)
				{
					delete f;
				}
				f = synthesiseN_1_N_2(ioPair, *iter1, *iter2, (OP)op, NONE, confidenceDegree);
				if (confidenceDegree > cd)
				{
					if (result)
					{
						delete result;
					}
					cd = confidenceDegree;
					result = f;
					return result;
				}
			}
		}
	}

	for (int op = PLUS; op <= R_DIV; op++)
	{
		for (TransformVector_t::const_iterator iter1 = TransPri.begin(); iter1 != TransPri.end(); iter1++)
		{
			if (Operation((OP)op).getPriority() == iter1->getPriority())
			{
				continue;
			}
			for (TransformVector_t::const_iterator iter2 = TransPri.begin(); iter2 != TransPri.end(); iter2++)
			{
				if (Operation((OP)op).getPriority() == iter2->getPriority())
				{
					continue;
				}
				for (OPVector_t::const_iterator iter = OPPri.begin(); iter != OPPri.end(); iter++)
				{
					if (f && result != f)
					{
						delete f;
					}
					f = synthesiseN_1_N_2(ioPair, *iter1, *iter2, (OP)op, *iter, confidenceDegree);
					if (confidenceDegree > cd)
					{
						if (result)
						{
							delete result;
						}
						cd = confidenceDegree;
						result = f;
						return result;
					}
				}
			}
		}
	}

	return result;
}

Function* Conjecture::nonspeedup_synthesise(bool enableN_1_N_2, const ioPair_t &ioPair, int &cd)
{
	Function* f = NULL;
	Function* result = NULL;
	cd = 0;
	int confidenceDegree = 0;

	if (ioPair.empty())
	{
		return NULL;
	}

	if (allSame(ioPair))
	{
		cd = ioPair.size();
		return new constFunction(ioPair.begin()->second);
	}

	double begin = ioPair.begin()->second;
	if (begin == 1.0 / zero || begin == -1.0 / zero || begin == 0.0 / zero)
	{
		return NULL;
	}

	f = processN_2(ioPair, confidenceDegree);
	if (confidenceDegree > cd)
	{
		if (result)
		{
			delete result;
		}
		cd = confidenceDegree;
		result = f;
	}

	f = processN_1(ioPair, confidenceDegree);
	if (confidenceDegree > cd)
	{
		if (result)
		{
			delete result;
		}
		cd = confidenceDegree;
		result = f;
	}

	if (enableN_1_N_2)
	{
		f = processN_1_N_2(ioPair, confidenceDegree);
		if (confidenceDegree > cd)
		{
			if (result)
			{
				delete result;
			}
			cd = confidenceDegree;
			result = f;
		}
	}

	return result;
}

Function* Conjecture::synthesise(const ioPair_t &ioPair, int &cd)
{
	unsigned group_num = MIN_GROUP;
	Function* result = NULL;
	cd = 0;
	int cur_cd;
	do
	{
		if (group_num < ioPair.size())
		{
			if(group_num < MAX_GROUP)
			{
				result = nonspeedup_synthesise(true, extract(ioPair, group_num), cur_cd);
			}
			else
			{
				result = simpleRecursion(ioPair, cd);
				break;
			}
		}
		else
		{
			group_num = ioPair.size();
			result = nonspeedup_synthesise(true, ioPair, cur_cd);
		}
		if (check(ioPair, result))
		{
			cd = cur_cd + ioPair.size() - group_num;
			break;
		}

		// check invalid
		cd = 0;
		if (result)
		{
			delete result;
			result = NULL;
		}
		group_num++;
	}while (group_num <= ioPair.size());

	return result;
}

Function* Conjecture::simpleRecursion(const ioPair_t &ioPair, int &cd)
{
	cd = 0;
	if (ioPair.empty())
	{
		return NULL;
	}

	if (allSame(ioPair))
	{
		cd = ioPair.size();
		return new constFunction(ioPair.begin()->second);
	}

	double begin = ioPair.begin()->second;
	if (begin == 1.0 / zero || begin == -1.0 / zero || begin == 0.0 / zero)
	{
		return NULL;
	}

	int input = ioPair.begin()->first;
	double output = ioPair.begin()->second;

	ioPair_t diff_ioPair = diffMapN_1(ioPair, PLUS);

	Function* diff_func = simpleRecursion(diff_ioPair, cd);

	Transform trans;
	trans.addBasicTrans(PLUS, *diff_func);
	delete diff_func;
	return new N_1Function(trans, input, output);
}
