#pragma once

// Conjecture.h

#include <map>
#include "GeneralTerm.h"

typedef map<int, double> ioPair_t;
typedef vector<Transform> TransformVector_t;
typedef vector<OP> OPVector_t;

const double zero = 0;

class Conjecture
{
public:
	Conjecture();
	Function* generalTerm(const ioPair_t &ioPair, int &cd);
	Function* synthesise(const ioPair_t &ioPair, int &cd);

private:
	bool allSame(const ioPair_t &ioPair);
	ioPair_t diffMapN_1(const ioPair_t &ioPair, const Operation &op);
	ioPair_t diffMapN_2(const ioPair_t &ioPair, const Operation &op);
	ioPair_t diffMapN_1_N_2(const ioPair_t &ioPair, const Transform &trans_1, const Transform &trans_2, const Operation &op, const Operation &newOp);

	ioPair_t extract(const ioPair_t &ioPair, unsigned &group_num);
	bool check(const ioPair_t &ioPair, Function* f);

	Function* synthesiseN_1(const ioPair_t &ioPair, const Operation &op, int &cd);
	Function* synthesiseN_2(const ioPair_t &ioPair, const Operation &op, int &cd);
	Function* synthesiseN_1_N_2(const ioPair_t &ioPair, Transform trans_1, Transform trans_2, const Operation &op, const Operation &newOp, int &cd);

	Function* processN_1(const ioPair_t &ioPair, int &cd);
	Function* processN_2(const ioPair_t &ioPair, int &cd);
	Function* processN_1_N_2(const ioPair_t &ioPair, int &cd);

	Function* nonspeedup_synthesise(bool enableN_1_N_2, const ioPair_t &ioPair, int &cd);

	Function* simpleRecursion(const ioPair_t &ioPair, int &cd);

	OPVector_t OPPri;
	TransformVector_t TransPri;
};
