#include <stdio.h>
#include<stdlib.h>
#include<string.h>

void MultiMatrixMultiply(int *p, int n, int **m, int **s);
//p：矩阵维度	n：矩阵个数		m：最优值		s：断点

int main(void)
{
	int n, i;
	printf("how many matrixs : ");
	scanf("%d", &n);

	int *dimension;
	dimension = (int*)malloc(sizeof(int)*(n+1));
	
	int **res, **bpoint;		
	//res: 结果  bpoint: 断点

	res = (int**)malloc(sizeof(int*)*n);
	bpoint = (int**)malloc(sizeof(int*)*n);
	for (i = 0; i < n; i++)
	{
		res[i] = (int*)malloc(sizeof(int)*n);
		memset(res[i], 0xFFFF, sizeof(int)*n);
		bpoint[i] = (int*)malloc(sizeof(int)*n);
	}
	
	if (dimension == NULL || res == NULL || bpoint == NULL)
	{
		printf("fail to malloc\n");
		exit(1);
	}	

	printf("input the dimensions: ");
	for (i = 0; i <= n; i++)
	{
		scanf("%d", &dimension[i]);
	}

	MultiMatrixMultiply(dimension, n, res, bpoint);
	printf("%d\n", res[0][n - 1]);
	
	free(dimension);
	dimension = NULL;
	for (i = 0; i < n; i++)
	{
		free(res[i]);
		res[i] = NULL;
		free(bpoint[i]);
		bpoint[i] = NULL;
	}
	free(res);
	res = NULL;
	free(bpoint);
	bpoint = NULL;

	return 0;
}

void MultiMatrixMultiply(int *p, int n, int **m, int **s)
{
	int i, j, k;
	int t, temp;
	for (i = 0; i < n; i++)
		(*(m + i))[i] = 0;

	for (k = 2; k <= n; k++)		//子问题规模
	{
		for (i = 0; i <= n - k; i++)
		{
			t = i + k -1;			//该子问题上界  t - i = k - 1
			m[i][t] = 0 + m[i + 1][t] + p[i] * p[i+1] * p[t+1];		//以第i个矩阵为断点
			s[i][t] = i;
			for (j = i + 1; j < t; j++)
			{
				temp = m[i][j] + m[j + 1][t] + p[i] * p[j + 1] * p[t + 1];
				if (temp < m[i][t])
				{
					m[i][t] = temp;
					s[i][t] = j;
				}
			}
		}
	}
}//MultiMatrixMultiply
