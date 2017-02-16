#include<iostream>
#include<cstdio>
#include<string>
#include<ctime>

#include"qsort.h"
#include"divide_sort_merge.h"

using namespace std;

//�����ļ�·��
char* configure(char *path,FILE*fp)
{
	//��fp��ȡһ�����ݵ�a
	char a[200];
	fgets(a, 1024, fp);

	//���� "=" ǰ������
	char*p = a;
	while (*p != '=')p++;
	p++;

	//�����ַ���
	char *road = path;
	int i = 0;
	while (*p != ';')*road++ = *p++;
	*road = '\0';

	return road;
}


int main()
{
	cout << "--------------------����ʼ-------------------" << endl << endl;

	double START = clock();
	cout << "��ȡ�����ļ���..." << endl<<endl;
	
	
	//��ȡ�����ļ�
	FILE*fp;
	if ((fp = fopen("Sort.param", "r")) == NULL)
		perror("���ļ�Sort.paramʧ�ܣ�");

	

	char input_path[200], output_path[200],
		num_thread_str[4], file_size_str[15], filenum_str[10],
		k1_str[5], k2_str[5];
	int num_thread, file_size, filenum,
		k1, k2;

	configure(input_path, fp);
	configure(output_path, fp);

	configure(num_thread_str, fp);
	num_thread = atoi(num_thread_str);

	configure(file_size_str, fp);
	file_size = atoi(file_size_str);

	configure(filenum_str, fp);
	filenum = atoi(filenum_str);

	configure(k1_str, fp);
	k1 = atoi(k1_str);

	configure(k2_str, fp);
	k2 = atoi(k2_str);

	int capacity = file_size / filenum;

	//�ر������ļ�
	fclose(fp);

	
	double start = clock();
	//�ָ��ļ�
	divide_file(input_path, filenum, capacity);
	double end = clock();
	cout << "�ָ��ļ�ʱ�䣺" << (end - start) / CLOCKS_PER_SEC << "s" << endl << endl;


	start = clock();
	//���߳�����
	cout << "�ڲ�������..." << endl;
	if(num_thread==1)
	  sort_all(filenum, capacity);
	else{
		//���߳�����
		int threadnum = num_thread;
		int number = 0, interval = filenum / threadnum;
		thread_group group;
		for (int i = 0; i < threadnum; i++)
		{
			group.create_thread(bind(&sort_n, number, number + interval - 1,capacity));
			number += interval;
		}
		group.join_all();
	}
	end = clock();
	cout << "�ڲ�����ʱ�䣺" << (end - start) / CLOCKS_PER_SEC << "s" << endl << endl;

	start = clock();
	//�ⲿ����
	external_sort(k1, k2, output_path,capacity);
	end = clock();
	cout << "�ⲿ����ʱ�䣺" << (end - start) / CLOCKS_PER_SEC <<"s"<< endl << endl;

	double END = clock();
	cout << "������ʱ�䣺 " << (END- START) / CLOCKS_PER_SEC <<"s"<< endl<<endl;
	
	cout << "--------------------�������-------------------" << endl<<endl;
	cout << "����q�˳�����" << endl;
	char c;
	cin >> c;
	if (c == 'q')return 0;
	else
	{
		cout << "���벻�Ϸ���" << endl;
		cout << "����q�˳�����" << endl;
	}


	return 0;
}

