#ifndef __FTOA__H_
#define __FTOA__H_

#include<cstdio>
#include<iostream>
using namespace std;

#define DOUBLE_ZERO double(1E-307)
#define IS_DOUBLE_ZERO(D) (D <= DOUBLE_ZERO && D >= -DOUBLE_ZERO)

static char*itoa(int n, char*chBuffer);
char*ftoaE(char*pchBuffer, int dppos, double value);

static char*itoa(int n, char*chBuffer)
{
	if (!chBuffer)return 0;

	//Process value sign
	if (n < 0)
	{
		n = -n;
		*chBuffer++ = '-';
	}
	else
		*chBuffer++ = '+';

	/*double i = 0.001, j = 1000.0;
	double N = (double)n;
	while (i != j)
	{
		i *= 10.0;
		j *= 0.1;
		int t = (int)N*i;
		*chBuffer++ = t + '0';

		N = (N*i - t)*j;
	}*/
	int i = 1000;
	while (i /= 10)
	{
		*chBuffer++ = n / i + '0';
		n %= i;
	}
	*chBuffer++ = '\n';
	return chBuffer;
}

//返回指向尾部后一位的指针
char*ftoaE(char*pchBuffer, int dppos, double value)
{
	double roundingValue = 0.5;
	int roundingPos = dppos;          
	double temp = value;
	int exp = 0;                    
	char *pch = pchBuffer;          

									//Process value sign
	if (value < 0.0)
	{
		value = -value;
		*pch++ = '-';
	}

	//得到指数
	if (!IS_DOUBLE_ZERO(value))
	{
		//// Get exponent of unrounded value for rounding
		temp = value;
		exp = 0;

		while (temp < 1.0)
		{
			temp *= 10.0;
			exp--;
		}
		while (temp >= 10.0)
		{
			temp *= 0.1;
			exp++;
		}

		roundingValue = 0.0000000005;

		temp += roundingValue;

		if (temp >= 10.0)
		{
			temp *= 0.1;
			exp++;
		}

		value = temp;
	}
	// Compose mantissa output string
	for (int i = roundingPos + 1; i > 0; i--)
	{
		int digital = (int)value;
		*pch++ = (char)(digital + '0');

		// Output decimal point
		if (i == roundingPos + 1)*pch++ = '.';

		value = (value - (double)digital)*10.0;
	}
	// Compose exponent output string
	*pch++ = 'E';

	char*tail = itoa(exp, pch);

	return tail;
}


#endif