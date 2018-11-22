#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<string.h>

#define PI 3.1416
#define EQUATOR_RADIUS 6378
#define INFINITY 60000

typedef struct
{
	int id;
	double dist;
	double lati;		//纬度
	double longti;		//经度
	double radLat, radLon;
}Data;

double Distance(Data *a, Data *b);
void QuickSort_Longti(Data *a, int low, int high);		//经度快速排序											
int Partition_Longti(Data *a, int low, int high);		//快速排序时的划分
void QuickSort_Lati(Data* a, int low, int high);		//纬度排序
int Partition_Lati(Data* a, int low, int high);		
void closest(Data *pri, int low, int high, Data& a, Data& b, double& min);

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
		data[i].radLat = data[i].lati * PI / 180;
		data[i].radLon = data[i].longti * PI / 180;
	}
	fclose(fp);

	Data a, b;
	double min = INFINITY;
	memset(&a, 0, sizeof(Data));
	memset(&b, 0, sizeof(Data));
	
	closest(data, 0, 1032, a, b, min);

	finish = clock();
	printf("%d\t%d\t%lf\t%lfs\n", a.id, b.id, min, (double)(finish - start) / CLOCKS_PER_SEC);
	return 0;
}


double Distance(Data *a, Data *b)
{ 
	return EQUATOR_RADIUS*acos(cos(a->radLat) * cos(b->radLat) * cos(a->radLon - b->radLon) + sin(a->radLat)*sin(b->radLat));
}//Distance

/**/
void closest(Data *pri, int low,int high, Data& a, Data& b, double& min)
{
	if (high - low <= 1)
		return;
	if (high - low == 2)
	{
		min = Distance(pri + low, pri + high);
		a = *(pri + low);
		b = *(pri + high);
		return;
	}
	QuickSort_Longti(pri, low, high);
	int m = (high + low) / 2;
	double  rmin = INFINITY;

	Data ra, rb;
	memset(&ra, 0, sizeof(Data));
	memset(&rb, 0, sizeof(Data));

	closest(pri, low, m, a, b, min);
	closest(pri, m + 1, high, ra, rb, rmin);

	if (rmin < min)
	{
		min = rmin;
		a = ra;
		b = rb;
	}

	int lcount = 0, rcount = 0;
	int i, j;
	double mark = (pri + m)->longti;
	Data *leftset, *rightset;

	//垂直分割线左边所有距离在dm之内的所有点组成的集合（按纬度排序）	
	for (i = m; i >= low; i--)
	{
		if ((pri + i)->longti + min > mark)
			lcount++;
		else
			break;
	}
	j = m - lcount + 1;

	leftset = (Data*)malloc(sizeof(Data)*lcount);
	for (i = 0; i < lcount; i++)
	{
		leftset[i] = *(pri + j);
		j++;
	}
	QuickSort_Lati(leftset, 0, lcount - 1);

	//垂直分割线右边所有距离在dm之内的所有点组成的集合（按纬度排序）
	for (i = m + 1; i <= high; i++)
	{
		if ((pri + i)->longti - min < mark)
			rcount++;
		else
			break;
	}
	j = m + 1;

	rightset = (Data*)malloc(sizeof(Data)*rcount);
	for (i = 0; i < rcount; i++)
	{
		rightset[i] = *(pri + j);
		j++;
	}
	QuickSort_Lati(rightset, 0, rcount - 1);

	double mmin = INFINITY;
	Data ma, mb;
	memset(&ma, 0, sizeof(Data));
	memset(&mb, 0, sizeof(Data));
	int pos = 0;	//rightset起始比较点下标
	double dis = 0;
	int temp = 0;
	for (i = 0; i < lcount; i++)
	{
		j = pos;
		temp = 0;
		for (; j < rcount; j++)
		{
			dis = fabs(EQUATOR_RADIUS*acos(cos(leftset[i].radLat) * cos(rightset[j].radLat) + sin(leftset[i].radLat)*sin(rightset[j].radLat)));
			
			if (dis > min && temp)
				break;

			if (dis > min)
				continue;

			if (dis < min && temp==0)
			{
				temp = 1;
				pos = j;
			}
			dis = Distance(leftset + i, rightset + j);
			if (dis < mmin)
			{
				mmin = dis;
				ma = leftset[i];
				mb = rightset[j];
			}
		}
	}
	free(leftset);
	free(rightset);
	leftset = NULL;
	rightset = NULL;
	if (mmin < min)
	{
		min = mmin;
		a = ma;
		b = mb;
	}
}//closest

/*
void closest(Data *pri, int low, int high, Data& a, Data& b, double& min)
{
	if (high - low <= 1)
		return;
	if (high - low == 2)
	{
		min = Distance(pri + low, pri + high);
		a = *(pri + low);
		b = *(pri + high);
		return;
	}
	QuickSort_Longti(pri, low, high);
	int m = (high + low) / 2;
	double  rmin = INFINITY;

	Data ra, rb;
	memset(&ra, 0, sizeof(Data));
	memset(&rb, 0, sizeof(Data));

	closest(pri, low, m, a, b, min);
	closest(pri, m + 1, high, ra, rb, rmin);

	if (rmin < min)
	{
		min = rmin;
		a = ra;
		b = rb;
	}

	int lcount = 0, rcount = 0;
	int i, j;
	double mark = (pri + m)->longti;

	Data leftset[800], rightset[800];
	//垂直分割线左边所有距离在dm之内的所有点组成的集合（按纬度排序）	
	for (i = m; i >= low; i--)
	{
		if ((pri + i)->longti + min > mark)
		{
			leftset[lcount++] = *(pri + i);
		}			
		else
			break;
	}
	j = m - lcount + 1;

	QuickSort_Lati(leftset, 0, lcount - 1);

	//垂直分割线右边所有距离在dm之内的所有点组成的集合（按纬度排序）
	for (i = m + 1; i <= high; i++)
	{
		if ((pri + i)->longti - min < mark)
			leftset[rcount++] = *(pri + i);
		else
			break;
	}
	j = m + 1;

	QuickSort_Lati(rightset, 0, rcount - 1);

	double mmin = INFINITY;
	Data ma, mb;
	memset(&ma, 0, sizeof(Data));
	memset(&mb, 0, sizeof(Data));
	int pos = 0;	//rightset起始比较点下标
	double dis = 0;
	int temp = 0;
	for (i = 0; i < lcount; i++)
	{
		j = pos;
		temp = 0;
		for (; j < rcount; j++)
		{
			dis = fabs(EQUATOR_RADIUS*acos(cos(leftset[i].radLat) * cos(rightset[j].radLat) + sin(leftset[i].radLat)*sin(rightset[j].radLat)));

			if (dis > min && temp)
				break;

			if (dis > min)
				continue;

			if (dis < min && temp == 0)
			{
				temp = 1;
				pos = j;
			}
			dis = Distance(leftset + i, rightset + j);
			if (dis < mmin)
			{
				mmin = dis;
				ma = leftset[i];
				mb = rightset[j];
			}
		}
	}

	if (mmin < min)
	{
		min = mmin;
		a = ma;
		b = mb;
	}
}//closest
*/

void QuickSort_Longti(Data* a, int low, int high)	//经度排序
{
	int i;
	for (i = low; i < high; i++)		//判断非递减
	{
		if ((a + i)->longti >(a + i + 1)->longti)
			break;
	}
	if (i == high)
		return;
	if (low < high)
	{
		int pivotloc = Partition_Longti(a, low, high);
		QuickSort_Longti(a, low, pivotloc - 1);
		QuickSort_Longti(a, pivotloc + 1, high);
	}
}//QuickSort_Longti

int Partition_Longti(Data* a, int low, int high)
{
	int i = low;
	int j = high + 1;
	Data key = *(a + low);
	Data temp;
	while (true)
	{
		while (  (a+ (++i))->longti < key.longti && i < high);
		while ((a + (--j))->longti > key.longti);
		if (i >= j)
			break;
		temp = *(a + i);
		*(a + i) = *(a + j);
		*(a + j) = temp;
	}
	*(a + low) = *(a + j);
	*(a + j) = key;
	return j;
}//Partition_Longti

void QuickSort_Lati(Data* a, int low, int high)	//经度排序
{
	int i;
	for (i = low; i < high; i++)		//判断非递减
	{
		if ((a + i)->lati >(a + i + 1)->lati)
			break;
	}
	if (i == high)
		return;
	if (low < high)
	{
		int pivotloc = Partition_Longti(a, low, high);
		QuickSort_Longti(a, low, pivotloc - 1);
		QuickSort_Longti(a, pivotloc + 1, high);
	}
}//QuickSort_Lati

int Partition_Lati(Data* a, int low, int high)
{
	int i = low;
	int j = high + 1;
	Data key = *(a + low);
	Data temp;
	while (true)
	{
		while ((a + (++i))->lati < key.lati && i < high);
		while ((a + (--j))->lati > key.lati);
		if (i >= j)
			break;
		temp = *(a + i);
		*(a + i) = *(a + j);
		*(a + j) = temp;
	}
	*(a + low) = *(a + j);
	*(a + j) = key;
	return j;
}//Partition_Lati
