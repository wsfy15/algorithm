#include<stdio.h>
#include<stdlib.h>

void MinWeightTriangle(int n, int *data, int **a, int **b);
// 最优三角剖分  n:边（顶点）个数  data:权值  a: 存放最优值  b: 断点

void PrintBreakPoint(int **b, int s, int e);
//打印断点

int main()
{
	int n, i;
	printf("number of edge:");
	scanf("%d", &n);

	int *data = (int*)malloc(sizeof(int)*(n+1));

	for (i = 0; i <= n; i++)
	{
		scanf("%d", &data[i]);
	}

	int **a = (int**)malloc(sizeof(int*)*n);
	int **b = (int**)malloc(sizeof(int*)*n);
	for (i = 0; i < n; i++)
	{
		a[i] = (int*)malloc(sizeof(int)*n);
		b[i] = (int*)malloc(sizeof(int)*n);
	}
	
	MinWeightTriangle(n, data, a, b);

	printf("min weight: %d\n", a[0][n - 1]);
	PrintBreakPoint(b, 0, n - 1);

	free(data);
	data = NULL;
	for (i = 0; i < n; i++)
	{
		free(a[i]);
		free(b[i]);
	}
	free(a);
	a = NULL;
	free(b);
	b = NULL;

	return 0;
}

void MinWeightTriangle(int n, int *data, int **a, int **b)
{
	int i, j, r, k;
	int temp;
	for (i = 0; i < n; i++)
	{
		a[i][i] = 0;
	}

	for (k = 2; k <= n; k++)
	{
		for (i = 0; i <= n - k; i++)
		{
			j = i + k - 1;
			a[i][j] = a[i + 1][j] + data[i] * data[i + 1] * data[j + 1];
			b[i][j] = i;
			for (r = i + 1; r < j; r++)
			{
				temp = a[i][r] +  a[r + 1][j] + data[i] * data[r + 1] * data[j + 1];
				if (temp < a[i][j])
				{
					a[i][j] = temp;
					b[i][j] = r;
				}
			}
		}
	}
}//MinWeightTriangle

void PrintBreakPoint(int **b, int s, int e)
{
	if (e - s <= 1)
		return;

	int mid = b[s][e];

	PrintBreakPoint(b, s, mid);
	PrintBreakPoint(b, mid + 1, e);

	printf("%d\n", mid);
}//PrintBreakPoint