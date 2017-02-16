#include<iostream>
#include<cstdio>
#include<string>
#include<ctime>

#include"qsort.h"
#include"divide_sort_merge.h"

using namespace std;

//解析文件路径
char* configure(char *path,FILE*fp)
{
	//从fp读取一行数据到a
	char a[200];
	fgets(a, 1024, fp);

	//忽略 "=" 前的名称
	char*p = a;
	while (*p != '=')p++;
	p++;

	//解析字符串
	char *road = path;
	int i = 0;
	while (*p != ';')*road++ = *p++;
	*road = '\0';

	return road;
}


int main()
{
	cout << "--------------------排序开始-------------------" << endl << endl;

	double START = clock();
	cout << "读取配置文件中..." << endl<<endl;
	
	
	//读取配置文件
	FILE*fp;
	if ((fp = fopen("Sort.param", "r")) == NULL)
		perror("打开文件Sort.param失败！");

	

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

	//关闭配置文件
	fclose(fp);

	
	double start = clock();
	//分割文件
	divide_file(input_path, filenum, capacity);
	double end = clock();
	cout << "分割文件时间：" << (end - start) / CLOCKS_PER_SEC << "s" << endl << endl;


	start = clock();
	//单线程排序
	cout << "内部排序中..." << endl;
	if(num_thread==1)
	  sort_all(filenum, capacity);
	else{
		//多线程排序
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
	cout << "内部排序时间：" << (end - start) / CLOCKS_PER_SEC << "s" << endl << endl;

	start = clock();
	//外部排序
	external_sort(k1, k2, output_path,capacity);
	end = clock();
	cout << "外部排序时间：" << (end - start) / CLOCKS_PER_SEC <<"s"<< endl << endl;

	double END = clock();
	cout << "总排序时间： " << (END- START) / CLOCKS_PER_SEC <<"s"<< endl<<endl;
	
	cout << "--------------------排序结束-------------------" << endl<<endl;
	cout << "输入q退出程序" << endl;
	char c;
	cin >> c;
	if (c == 'q')return 0;
	else
	{
		cout << "输入不合法！" << endl;
		cout << "输入q退出程序" << endl;
	}


	return 0;
}

