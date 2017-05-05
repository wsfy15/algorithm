#include <stdio.h>
#include <string.h>
#include<stdlib.h>

int MaxSum(int* data, int n, int& s, int& e);	
//求最大字段和	s:起点 e:终点

int main()
{
	int n, i;
	printf("length of array:");
	scanf("%d", &n);

	int *data = (int*)malloc(sizeof(int)*n);
	if (data == NULL)
	{
		printf("fail to malloc\n");
		exit(1);
	}
	for (i = 0; i < n; i++)
	{
		scanf("%d", &data[i]);
	}
	int s, e;
	int ans = MaxSum(data, n, s, e);
	printf("sum: %d s:%d e:%d\n", ans, s, e);

	free(data);
	data = NULL;

	return 0;
}

int MaxSum(int* data, int n, int& s, int& e)
{
	int i, temp;
	int b = 0, sum = 0;
	for (i = 0; i < n; i++)
	{
		if (b > 0)
		{
			b += data[i];
		}			
		else 
		{
			temp = i;
			b = data[i];
		}
		if (b > sum)
		{
			s = temp;
			e = i;
			sum = b;
		}
	}

	return sum;
}//MaxSum
