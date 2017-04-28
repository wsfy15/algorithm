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
	double lati;		//γ��
	double longti;		//����
	double radLat, radLon;
}Data;

double Distance(Data *a, Data *b);						//���������
void QuickSort_Longti(Data *a, int low, int high);		//���ȿ�������											
int Partition_Longti(Data *a, int low, int high);		//��������ʱ�Ļ���
void QuickSort_Lati(Data* a, int low, int high);		//γ������
int Partition_Lati(Data* a, int low, int high);		
void closest(Data *pri, int low, int high, Data& a, Data& b, double& min);//����ӽ����

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
		printf("fail to open data.txt\n");
		exit(1);
	}

	for (i = 0; i < 1033; i++)
	{
		fscanf(fp, "%d %lf %lf %lf", &data[i].id, &data[i].longti, &data[i].lati, &data[i].dist);
		data[i].radLat = data[i].lati * PI / 180;
		data[i].radLon = data[i].longti * PI / 180;
	}
	fclose(fp);
	Data data_temp[1050];
	memcpy(data_temp, data, sizeof(Data) * 1050);

	//����ӽ����
	Data a, b;
	double min = INFINITY;
	memset(&a, 0, sizeof(Data));
	memset(&b, 0, sizeof(Data));
	
	closest(data, 0, 1032, a, b, min);
	finish = clock();
	printf("��ӽ����:\n");
	printf("a.id: %d\tb.id: %d\tdistance: %.9lfkm\n", a.id, b.id, min);
	printf("time: %lfs\n", (double)(finish - start) / CLOCKS_PER_SEC);

	//��νӽ����
	start = clock();
	Data ra, rb;
	double rmin = INFINITY;	
	memset(&ra, 0, sizeof(Data));
	memset(&rb, 0, sizeof(Data));
	

	for (i = 0; i < 1033; i++)
	{
		if (data[i].id == a.id)
		{
			data[i] = data[1032];		//��A�Ƴ�Ҫ�㼯��
			break;
		}
	}
	closest(data, 0, 1031, ra, rb, rmin);

	for (i = 0; i < 1033; i++)
	{
		if (data_temp[i].id == b.id)
		{
			data_temp[i] = data_temp[1032];		//��B�Ƴ�Ҫ�㼯��
			break;
		}
	}
	memset(&a, 0, sizeof(Data));
	memset(&b, 0, sizeof(Data));
	min = INFINITY;
	closest(data_temp, 0, 1031, a, b, min);

	if (rmin < min)
	{
		min = rmin;
		a = ra;
		b = rb;
	}

	finish = clock();
	printf("����ӽ����:\n");
	printf("a.id: %d\tb.id: %d\tdistance: %.9lfkm\n", a.id, b.id, min);
	printf("time: %lfs\n", (double)(finish - start) / CLOCKS_PER_SEC);
	return 0;
}


double Distance(Data *a, Data *b)
{ 
	return EQUATOR_RADIUS*acos(cos(a->radLat) * cos(b->radLat) * cos(a->radLon - b->radLon) + sin(a->radLat)*sin(b->radLat));
}//Distance


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

	//�ֱ����е����ҵ���ӽ����
	closest(pri, low, m, a, b, min);			
	closest(pri, m + 1, high, ra, rb, rmin);

	if (rmin < min)
	{
		min = rmin;
		a = ra;
		b = rb;
	}

	int i, j;
	double mark = (pri + m)->longti;

	Data leftset[800], rightset[800];
	/*
		��ָ��+malloc+free��ʱ�򣬱��������
		������Ļ�������ʱ�����ᵼ��ʱ�俪��̫��
	*/

	//��ֱ�ָ���������о�����dm֮�ڵ����е���ɵļ��ϣ���γ������
	int lcount = 0;
	for (i = m; i >= low; i--)
	{
		if ((pri + i)->longti + min > mark)
		{
			leftset[lcount++] = *(pri + i);
		}			
		else
			break;
	}
	QuickSort_Lati(leftset, 0, lcount - 1);

	//��ֱ�ָ����ұ����о�����dm֮�ڵ����е���ɵļ��ϣ���γ������
	int rcount = 0;
	for (i = m + 1; i <= high; i++)
	{
		if ((pri + i)->longti - min < mark)
			rightset[rcount++] = *(pri + i);
		else
			break;
	}
	QuickSort_Lati(rightset, 0, rcount - 1);

	double mmin = INFINITY;
	Data ma, mb;
	memset(&ma, 0, sizeof(Data));
	memset(&mb, 0, sizeof(Data));

	int pos = 0;		//rightset��ʼ�Ƚϵ��±�
	double dis = 0;	
	int temp = 0;
	for (i = 0; i < lcount; i++)
	{
		j = pos;
		temp = 0;		//��¼����Ǹ�����ұߵĵ�Ĵ�ֱγ�Ⱦ����Ƿ�С��֮ǰ�����min
		for (; j < rcount; j++)
		{
			dis = fabs(EQUATOR_RADIUS*acos(cos(leftset[i].radLat) * cos(rightset[j].radLat) + sin(leftset[i].radLat)*sin(rightset[j].radLat)));
			//����Ĵ�ֱγ�Ⱦ��루���辭����ͬ��
			if (dis > min && temp)		//����ĵ㲻Ҫ������
				break;

			if (dis > min)				//����ĵ㻹��Ҫ����
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


void QuickSort_Longti(Data* a, int low, int high)	//��������
{
	int i;
	for (i = low; i < high; i++)		//�жϷǵݼ�
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

void QuickSort_Lati(Data* a, int low, int high)	//��������
{
	int i;
	for (i = low; i < high; i++)		//�жϷǵݼ�
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
