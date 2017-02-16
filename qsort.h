#ifndef __QSORT__H_
#define __QSORT__H_

static void swap(double a[], int i, int j);
static int median3(double arr[], int low, int high);
static int arrange(double arr[], int l, int r, double p);
void qsort(double arr[], int l, int r);

//交换
static void swap(double a[], int i, int j)
{
	double tmp = a[i];
	a[i] = a[j];
	a[j] = tmp;
}

//三数取中
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

//比轴值大的放右边，比轴值小的放左边
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
 void qsort(double arr[], int l, int r)//l指向第一个，r指向最后一个
{
	if (l >= r)return;
	int index = median3(arr, l, r);
	//把轴值放到最后
	swap(arr, index, r);
	int pivot = arrange(arr, l - 1, r, arr[r]);
	swap(arr, pivot, r);

	qsort(arr, l, pivot - 1);
	qsort(arr, pivot + 1, r);
}


#endif