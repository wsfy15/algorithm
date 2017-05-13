#include<stdio.h>
#include<stdlib.h>

void Knapsack(int n, int max_weight, int *weight, int *value, int **ans);
//ans[子问题(  - n-1)][质量]

int main()
{
	int i, n, max_weight;

	FILE *fp;
	fp = fopen("data1.txt", "r");
	if (fp == NULL)
	{
		printf("fail to open file\n");
		exit(1);
	}
	fscanf(fp, "%d", &max_weight);
	n = 0;
	while (!feof(fp))
	{
		n++;
		fscanf(fp, "%d", &i);
	}
	n /= 2;
	int *weight = (int*)malloc(sizeof(int)*(n+1));
	int *value = (int*)malloc(sizeof(int)*(n+1));

	fseek(fp, SEEK_SET, 0);
	fscanf(fp, "%d", &i);
	for (i = 1; i <= n; i++)
	{
		fscanf(fp, "%d", &weight[i]);
	}
	for (i = 1; i <= n; i++)
	{
		fscanf(fp, "%d", &value[i]);
	}

	int **ans = (int**)malloc(sizeof(int) * (n+1));
	for (i = 0; i <= n; i++)
	{
		ans[i] = (int*)malloc(sizeof(int) * (max_weight+1));
	}

	Knapsack(n, max_weight, weight, value, ans);
	printf("max weight: %d\n", ans[1][max_weight]);

	for (i = 2; i <= n; i++)
	{
		if (ans[i][n - 1] < ans[i - 1][n - 1])
		{
			printf("%d\tweight:%d\tvalue:%d\n", i-1, weight[i - 1], value[i-1]);
		}
	}

	for (i = 0; i <= n; i++)
	{
		free(ans[i]);
	}
	free(ans);
	ans = NULL;

	return 0;
}

void Knapsack(int n, int max_weight, int *weight, int *value, int **ans)
{
	int i, j;
	for (j = 1; j <= max_weight; j++)
	{
		ans[n][j] = (j >= weight[n]) ? value[n] : 0;
		for (i = n - 1; i >= 1; i--)
		{
			if (j < weight[i])
			{
				ans[i][j] = ans[i + 1][j];
			}
			else
			{
				if (ans[i + 1][j - weight[i]] + value[i] > ans[i + 1][j])
				{
					ans[i][j] = ans[i + 1][j - weight[i]] + value[i];
				}
				else
				{
					ans[i][j] = ans[i + 1][j];
				}
			}
		}
	}
}//Knapsack
