#include <cstdlib>
#include <cstring>
#include <cstdbool>

typedef unsigned long long uint64;
typedef long long int64;
typedef unsigned short uint16;

static void sort_word(double *array, size_t n, int word, double *buf)
{

	int *cnt = new int[65536];
	//归零
	for (int i = 0; i < 65536; i++)
		cnt[i] = 0;
	//计数
	for (size_t i = 0; i < n; i++)
	{
		uint16 tmp = ((uint16 *)(array + i))[word];
		cnt[tmp]++;
	}
	//计算起始位置
	for (size_t i = 1; i < 65536; i++)
	{
		cnt[i] += cnt[i - 1];
	}

	memcpy(buf, array, sizeof(double)*n);

	for (int i = (int)n - 1; i >= 0; i--)
	{
		uint16 tmp = ((uint16 *)(buf + i))[word];
		array[cnt[tmp] - 1] = buf[i];
		cnt[tmp]--;
	}

	delete[]cnt;
}

void fsort(double *array, size_t n)
{
	
	double *buf = (double *)malloc(sizeof(double)*n);

	sort_word(array, n, 0, buf);
	sort_word(array, n, 1, buf);
	sort_word(array, n, 2, buf);
	sort_word(array, n, 3, buf);

	memcpy(buf, array, sizeof(double)*n);

	for (int i = (int)n - 1; i >= 0; i--)
	{
		if (((int64 *)buf)[i] < 0)
		{
			array[n - i - 1] = buf[i];
		}
		else
		{
			memcpy(array + n - i - 1, buf, sizeof(double)*(i + 1));
			break;
		}
	}

	free(buf);
}