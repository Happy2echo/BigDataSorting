#ifndef __STRTOD__H_
#define __STRTOD__H_

#include <cctype>
#include<iostream>
using namespace std;

#define ZERO double(1E-307)
#define IS_ZERO(D) (D <= ZERO && D >= -ZERO)

static bool my_strtod(const char*s, char**endptr, double&result);
static char* get_wrong_data(char*p);
int parse_to_double(double *a, const int capacity, char*&addr, char*pnext, int&wrongNum);

//将字符串解析成double
//若遇到异常字符，返回0.0,且endpter == s
//无异常，返回转化成果的double值
static bool my_strtod(const char*s, char**endptr, double&result)
{
	register const char*p = s;
	register  double value = 0.0;
	int sign = 0;
	double factor;
	int expo;
	int iplace = 0;


	while (isspace(*p))p++;
	char*q = (char*)p;

	if (*p == '-' || *p == '+')sign = *p++;


	while ('0' <= *p  &&  *p <= '9')
	{
		value = value * 10 + (*p++ - '0');
		iplace++;
	}

	//如果表达方式如122.234
	factor = 0.0;
	if (*p == '.')
	{
		factor = 1.0;
		p++;
		while ('0' <= *p  &&  *p <= '9')
		{
			factor *= 0.1;
			value += ((*p++ - '0')*factor);
			iplace++;
		}
	

	}

	//处理十进制转二进制偏小的情况
	
	char* tmp = (char*)p;
	tmp--;
	//如果形如x.xxxxxxxxx5000
	if (!IS_ZERO(factor))
	{
		while (*tmp == '0') { tmp--; iplace--; }
	}
	if (iplace == 11 && *tmp == '5')
	{
		if (!IS_ZERO(factor))
			value += factor;
		else
			value++;
	}	


	
	//如果如：1.23456E+3
	if (*p == 'E' || *p == 'e')
	{
		expo = 0;
		factor = 10.0;

		switch (*++p)
		{
		case'-':
			factor = 0.1;
		case'+':
			p++;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			break;
		default:
			value = 0.0;
			p = s;
			goto done;

		}

		while ('0' <= *p && *p <= '9')
			expo = 10 * expo + (*p++ - '0');

		while (1)
		{
			if (expo & 1)
				value *= factor;
			if ((expo >>= 1) == 0)
				break;
			factor *= factor;
		}

	}


done:
	if (endptr != 0)
		*endptr = (char*)p;

	/*if (*endptr != q)
	{
		if (sign == '-')
			result = -value;
		else
			result = value;
		return true;
	}
	else  return false;*/
	if (**endptr == '\r' || **endptr == '\n' || **endptr == '\0')
	{
		if (sign == '-')
			result = -value;
		else
			result = value;
		return true;
	}
	else
	{
		*endptr = (char*)q;
		return false;
	}

}


//把input指向的非字符放到buffer中
//以\n结束
//返回指向\n后一位的指针
static char* get_wrong_data(char*p)
{
	char temp[500];
	while (isspace(*p))p++;

	int i = 0;
	while (*p != '\n' &&*p != '\0')
		temp[i++] = *p++;

	temp[i++] = '\n';
	temp[i] = '\0';

	cout <<"非法条目： "<< temp;

	p++;
	return p;
}


//解析函数
//double*a  数据解析到a
//capacity  总共解析capacity条数目
//addr      待解析数据的首地址
//wrongNum  非法条目数目
//返回成功解析的double的个数
int parse_to_double(double *a, const int capacity, char*&addr, char*pnext, int&wrongNum)
{
	int i = capacity;
	int j = 0;
	while (i--)
	{
		if (my_strtod(addr, &pnext, a[j]))
		{
			j++;
			addr = pnext;
		}
		else
		{
			addr = get_wrong_data(addr);
			wrongNum++;
		}
	}

	return j;
}



#endif