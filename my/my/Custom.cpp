#include "StdAfx.h"

#include <cmath>
#include "Custom.h"

Custom::Custom(void)
{
	f_name = "f";
}

int Custom::f(int x)
{
	int count = 0;
	int num = 2;
	while(count < x)
	{
		num++;
		bool isprime = true;
		for(int i = 2; i <= sqrt(float(num)); i++)
		{
			if(num / i * i == num)
			{
				isprime = false;
				break;
			}
		}
		if(isprime)
		{
			count++;
		}
	}
	return num;
}

