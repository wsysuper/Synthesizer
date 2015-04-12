// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

inline string dtos(double d)
{
	ostringstream stream;
	stream << d;
	return stream.str();
}

inline string dtos(double d, double omit)
{
	if (d == omit)
	{
		return "";
	}
	return dtos(d);
}

inline double round(double d)
{
	return floor(d * 1e10 + 0.5) / 1e10;
}
