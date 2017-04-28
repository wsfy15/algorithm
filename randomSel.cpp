#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef struct
{
	int id;
	double dist, lati, longti;
}Data;

int RandomizedSelect(Data a[], int low, int high, int k);//线性选择
void BubbleSort(Data a[], int low, int high);			//冒泡排序
int Partition(Data a[], int low, int high);				//划分

int count;

int main(void)
{
	int i;
	long long int start, finish;
	start = clock();
	Data data[1050];

	FILE* fp;
	fp = fopen("data.txt", "r");
	if (fp == NULL)
	{
		printf("fail to open");
		exit(1);
	}

	for (i = 0; i < 1033; i++)
	{
		fscanf(fp, "%d %lf %lf %lf", &data[i].id, &data[i].longti, &data[i].lati, &data[i].dist);
	}
	fclose(fp);

	count = 0;
	int id = RandomizedSelect(data, 0, 1032, 1);
	printf("k-dist最小的点的id：\t%d，距离：\t%lf\t递归层数:%d\n", data[id].id, data[id].dist,count);
	
	count = 0;
	id = RandomizedSelect(data, 0, 1032, 5);
	printf("k-dist第5小的点的id：\t%d，距离：\t%lf\t递归层数:%d\n", data[id].id, data[id].dist, count);
	
	count = 0;
	id = RandomizedSelect(data, 0, 1032, 50);
	printf("k-dist第50小的点的id：\t%d，距离：\t%lf\t递归层数:%d\n", data[id].id, data[id].dist, count);
	
	count = 0;
	id = RandomizedSelect(data, 0, 1032, 1033);
	printf("k-dist最大的点的id：\t%d，距离：\t%lf\t递归层数:%d\n", data[id].id, data[id].dist, count);
	finish = clock();
	printf("time: %lfs\n", (double)(finish - start) / CLOCKS_PER_SEC);
	return 0;
}

int RandomizedSelect(Data a[], int low, int high, int k)
{
	count++;
	if (low == high)
		return low;		
	if (high - low < 10)		//元素少，直接排序输出
	{
		BubbleSort(a, low, high);
		return low + k - 1;
	}

	int i, j, s;
	Data temp;
	for (i = 0; i <= (high - low - 4) / 5; i++)			//分成大小为5的子序列，将其中位数交换出来
	{
		s = low + 5 * i;
		BubbleSort(a, s, s + 4);
		temp = a[low + i];
		a[low + i] = a[s + 2];
		a[s + 2] = temp;
	}

	int pos = RandomizedSelect(a, low, low + (high - low - 4) / 5, (high - low + 6) / 10);
	count--;

	if (pos != low)
	{
		temp = a[pos];
		a[pos] = a[low];
		a[low] = temp;
	}

	i = Partition(a, low, high);
	j = i - low + 1;		//左端元素个数

	if (k == j)
		return i;
	if (k < j)
		return RandomizedSelect(a, low, i - 1, k);
	else
		return RandomizedSelect(a, i + 1, high, k - j);
}//randomizedSelect

void BubbleSort(Data a[], int low, int high)		//0  -  n
{
	int change = 1;
	int i;
	Data temp;

	while (low <= high && change)
	{
		change = 0;
		for (i = low; i < high; i++)
			if (a[i].dist > a[i + 1].dist)
			{
				temp = a[i];
				a[i] = a[i + 1];
				a[i + 1] = temp;
			}
		high--;
		for (i = high; i > low; i--)
			if (a[i].dist < a[i - 1].dist)
			{
				temp = a[i];
				a[i] = a[i - 1];
				a[i - 1] = temp;
				change = 1;
			}
		low++;
	}
}//BubbleSort

int Partition(Data a[], int low, int high)
{
	int i = low;
	int j = high + 1;
	Data key = a[low];
	Data temp;
	while (true)
	{
		while (a[++i].dist < key.dist && i < high);
		while (a[--j].dist > key.dist);
		if (i >= j)
			break;
		temp = a[i];
		a[i] = a[j];
		a[j] = temp;
	}
	a[low] = a[j];
	a[j] = key;

	return j;
}//Partition
