#ifndef __DIVIDE_SORT_MERGE__H_
#define __DIVIDE_SORT_MERGE__H_


#include<iostream>
#include<stdio.h>
#include<algorithm>
#include<Windows.h>
#include <ctime>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

#include"ftoaE.h"
#include"strtod.h"
#include"fsort.h"




using namespace std;
using namespace boost::interprocess;
using namespace boost;

//�ָ�
string new_file_name(char prefix[], int n);
void check_fp(FILE *fp);
void divide_file(const char*input_path, const int filenum, const int capacity);
//����
void sort_file(char* in_file,const int capacity);
void sort_all(const int filenum, const int capacity);
void sort_n(int s, int e, const int capacity);
//����
void adjust(int ls[], double b[], int k, int s);
void create_loser_tree(int ls[], double b[], int k);
void k_merge_binary_write(int ls[], double b[], int k, FILE** fp_in, FILE*fp_out);
void k_merge_format_write(int ls[], double b[], int k, FILE** fp_in, FILE*fp_out, const int capacity);
void external_sort(const int k1, const int k2, char* out_path, const int capacity);


string new_file_name(char prefix[], int n)
{
	char file[15], digital[6];
	strcpy(file, prefix);
	sprintf(digital, "%d", n);
	strcat(file, digital);
	return file;
}


void check_fp(FILE *fp)
{
	if (fp == NULL)
		perror("Error opening file");

}

// divide file
void divide_file(const char*input_path,const int filenum,const int capacity)
{
	printf("�ָ��ļ���...\n");

	//�����ڴ�ӳ��
	file_mapping m_file(input_path, read_only);
	mapped_region region(m_file, read_only);
	char * addr = (char*)region.get_address(),
		 *pnext=NULL;

	char pre[6] = "in.";
	int wrongNum = 0;               //��¼�쳣���ݵ���Ŀ
	int nfile = 0;

	for (int j = filenum-1; j >=0; j--, nfile++)
	{
		double *a = new double[capacity];

		
		int out_size = parse_to_double(a, capacity, addr, pnext, wrongNum);

		
		FILE* fp = fopen(new_file_name(pre, j).c_str(), "wb");
		check_fp(fp);

		//д��
		fwrite(a, sizeof(double), out_size, fp);
		
		fclose(fp);
		delete[]a;

		if (nfile == 100) {
			printf("�ָ��ļ���...��ȴ�...\n");
			nfile = 0;
		}
	}

	//������ڷǷ���Ŀ������Ƿ���Ŀ����
	if (wrongNum != 0)
		cout << "�Ƿ���Ŀ����Ϊ��" << wrongNum << endl;

	cout << "�ָ��ļ�����" << endl;
}


void sort_file(char* in_file,const int capacity)
{
	double* a = new double[capacity];
	FILE *fp1;
	
	fp1 = fopen(in_file, "rb+");
	check_fp(fp1);

	fseek(fp1, 0, SEEK_END);
	int in_size = ftell(fp1) / sizeof(double);

	
	fseek(fp1, 0, SEEK_SET);
	fread(a, sizeof(double), in_size, fp1);
	
	fsort(a, in_size);

	fseek(fp1, 0, SEEK_SET);
	fwrite(a, sizeof(double), in_size, fp1);

	fclose(fp1);
	
	delete[]a;
}

void sort_all(const int filenum,const int capacity)//���������ļ�
{
	char in_file[10] = "in.",
		digit[5];
	for (int i = 0; i<filenum; ++i)
	{
		sprintf(digit, "%d", i);
		strcat(in_file, digit);
		sort_file(in_file,capacity);
		in_file[3] = '\0';
	}
}

void sort_n(int s, int e,const int capacity)
{
	cout << "������..." << endl;
	char in_file[10] = "in.",
		
		digit[5];

	for (int i = s; i <= e; ++i)
	{
		sprintf(digit, "%d", i);
		strcat(in_file, digit);
		sort_file(in_file,capacity);
		in_file[3] = '\0';
	}
}

/**************************�ⲿ����(������)***********************/
void adjust(int ls[], double b[], int k, int s)
{
	int i, t;
	t = (s + k) >> 1;
	while (t>0)
	{
		if (b[s]>b[ls[t]])
		{
			i = s;
			s = ls[t];
			ls[t] = i;
		}
		t=t >> 1;
	}
	ls[0] = s;
}

void create_loser_tree(int ls[], double b[], int k)
{
	b[k] = (numeric_limits<double>::lowest)();
	int i;
	for (i = 0; i<k; i++)
		ls[i] = k;
	for (int i = k - 1; i >= 0; i--)
		adjust(ls, b, k, i);
}

void k_merge_binary_write(int ls[], double b[], int k, FILE** fp_in, FILE*fp_out)
{

	//�ֱ��K���ⲿ�ļ������һ��ֵ
	for (int i = 0; i < k; ++i)
		fread(&b[i], sizeof(double), 1, fp_in[i]);


	//����������
	create_loser_tree(ls, b, k);

	while (b[ls[0]] != (numeric_limits<double>::max)())
	{

		int q = ls[0];
		fwrite(b + q, sizeof(double), 1, fp_out);

		if (!fread(&b[q], sizeof(double), 1, fp_in[q]))
			b[q] = (numeric_limits<double>::max)();

		adjust(ls, b, k, q);
	}
}

void k_merge_format_write(int ls[], double b[], int k, FILE** fp_in, FILE*fp_out, const int capacity)
{

	
	for (int i = 0; i < k; ++i)
		fread(&b[i], sizeof(double), 1, fp_in[i]);


	//����������
	create_loser_tree(ls, b, k);

	
	char*buffer = new char[18 * capacity + 5];
	int size = capacity;
	char*p = buffer;

	int line = 0;
	while (b[ls[0]] != (numeric_limits<double>::max)())
	{
		if (line == 20000000)
		{
			cout << "�鲢�У���ȴ�..." << endl;
			line = 0;
		}
		//������������˾�������ļ�
		if (size == 0)
		{
			//output
			*p = '\0';
			fputs(buffer, fp_out);
			//update
			size = capacity;
			*buffer = '\0';
			p = buffer;
		}

		int q = ls[0];
		//fprintf(fp_out, "%.9e\n", b[q]);//����ǰ��Сֵ���������ļ���

		//�����������
		p = ftoaE(p, 9, b[q]);
		size--;
		line++;

		if (!fread(&b[q], sizeof(double), 1, fp_in[q]))
			b[q] = (numeric_limits<double>::max)();

		adjust(ls, b, k, q);
	}

	*p = '\0';
	fputs(buffer, fp_out);
	//�ͷŻ�����
	delete[]buffer;
}



void external_sort(const int k1, const int k2, char* out_path,const int capacity)
{
	cout << "�ⲿ������..." << endl;

	cout << "һ���鲢��..." << endl;
	int j = 0, i = 0;
	for (; j < k2; j++)
	{
		FILE* fp[100];
		FILE*fp_out;
		char pfx_in[15] = "in.", pfx_out[15] = "merge.";

		printf("�鲢in.%d��%d���ļ���...\n", i, (j + 1)*k1 - 1);
		//open input file
		int begin = i;
		for (int s = k1-1; s>=0; s--, i++)
		{
			fp[s] = fopen(new_file_name(pfx_in, i).c_str(), "rb");
			check_fp(fp[s]);
		}

		//open output file
		fp_out = fopen(new_file_name(pfx_out, j).c_str(), "wb");
		check_fp(fp_out);

		int ls[200];
		double b[201];

		//merge
		k_merge_binary_write(ls, b, k1, fp, fp_out);

		for (int c = 0; c<k1; c++)
			fclose(fp[c]);
		fclose(fp_out);

		//delete
		for (int s = k1 - 1; s >= 0; s--, begin++)
		{
			if (remove(new_file_name(pfx_in, begin).c_str())!= 0)
				perror("Error deleting file!");
		}


	}

	printf("һ���鲢����...\n");

	printf("�����鲢��...\n");

	FILE* fp_in[100];
	FILE*fp_out;
	char pfx_in[15] = "merge.";

	for (j = 0; j < k2; j++)
	{
		fp_in[j] = fopen(new_file_name(pfx_in, j).c_str(), "rb");
		check_fp(fp_in[j]);
	}

	fp_out = fopen(out_path, "w");
	check_fp(fp_out);

	int ls[200];
	double b[201];

	//merge
	k_merge_format_write(ls, b, k2, fp_in, fp_out,capacity);

	//close
	for (int c = 0; c<k2; c++)
		fclose(fp_in[c]);
	fclose(fp_out);

	//delete
	for (j = 0; j < k2; j++)
	{
		if (remove(new_file_name(pfx_in, j).c_str()) != 0)
			perror("Error deleting file!");
		
	}

	//cout << "�����鲢����..." << endl;
	printf("�����鲢����...\n");

	//cout << "���������" << endl;
	printf("���������\n");

}


#endif