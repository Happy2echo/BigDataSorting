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

//���ַ���������double
//�������쳣�ַ�������0.0,��endpter == s
//���쳣������ת���ɹ���doubleֵ
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

	//�����﷽ʽ��122.234
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

	//����ʮ����ת������ƫС�����
	
	char* tmp = (char*)p;
	tmp--;
	//�������x.xxxxxxxxx5000
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


	
	//����磺1.23456E+3
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


//��inputָ��ķ��ַ��ŵ�buffer��
//��\n����
//����ָ��\n��һλ��ָ��
static char* get_wrong_data(char*p)
{
	char temp[500];
	while (isspace(*p))p++;

	int i = 0;
	while (*p != '\n' &&*p != '\0')
		temp[i++] = *p++;

	temp[i++] = '\n';
	temp[i] = '\0';

	cout <<"�Ƿ���Ŀ�� "<< temp;

	p++;
	return p;
}


//��������
//double*a  ���ݽ�����a
//capacity  �ܹ�����capacity����Ŀ
//addr      ���������ݵ��׵�ַ
//wrongNum  �Ƿ���Ŀ��Ŀ
//���سɹ�������double�ĸ���
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