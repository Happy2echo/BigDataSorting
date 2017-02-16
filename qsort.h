#ifndef __QSORT__H_
#define __QSORT__H_

static void swap(double a[], int i, int j);
static int median3(double arr[], int low, int high);
static int arrange(double arr[], int l, int r, double p);
void qsort(double arr[], int l, int r);

//����
static void swap(double a[], int i, int j)
{
	double tmp = a[i];
	a[i] = a[j];
	a[j] = tmp;
}

//����ȡ��
static int median3(double arr[], int low, int high)
{
	int mid = (high + low) / 2;
	if (arr[mid] > arr[high])
		swap(arr, mid, high);
	if (arr[low] > arr[high])
		swap(arr, low, high);
	if (arr[mid] > arr[low])
		swap(arr, mid, low);

	//arr[mid] <= arr[low] <= arr[high]  
	return low;
}

//����ֵ��ķ��ұߣ�����ֵС�ķ����
static int arrange(double arr[], int l, int r, double p)
{
	do {
		while (arr[++l] < p);
		while (l<r&&arr[--r] > p);
		swap(arr, l, r);
	} while (l < r);
	return l;
}


//quick sort
 void qsort(double arr[], int l, int r)//lָ���һ����rָ�����һ��
{
	if (l >= r)return;
	int index = median3(arr, l, r);
	//����ֵ�ŵ����
	swap(arr, index, r);
	int pivot = arrange(arr, l - 1, r, arr[r]);
	swap(arr, pivot, r);

	qsort(arr, l, pivot - 1);
	qsort(arr, pivot + 1, r);
}


#endif