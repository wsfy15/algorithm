#include<stdio.h>
#include<stdlib.h>


/*
k = 3, n =8
初始	
1 2 3 4 5 6 7 8 
第一次（4次赋值）
1 2		3 4		5 6		7 8
2 1		4 3		6 5		8 7

12 34 56 78
21 43 65 87

第二次（2次赋值）
12		34		56		78
21		43		65		87
34		12		78		56
43		21		87		65

第三次（1次赋值）
56 78	12 34
65 87	21 43
78 56	34 12
87 54	43 21
*/

void table(int k, int n, int **a)	//循环赛日程表
{
	int i, j, t, s;
	for (i = 0; i < n; i++)
		a[0][i] = i + 1;

	int m = 1;		//记录搞定了前m行	

	for (s = 0; s < k; s++)		//总共需要进行k次操作
	{
		n /= 2;
		for (t = 0; t < n; t++)	//每次操作赋值的块数为n
		{
			for (i = m ; i < 2 * m; i++)//控制行  
			{
				for (j = m; j < 2 * m; j++)//控制列  
				{
					a[i][j + t *m * 2] = a[i - m][j + t *m * 2 - m];//右下角等于左上角的值  
					a[i][j + t *m * 2 - m] = a[i - m][j + t *m * 2];//左下角等于右上角的值  
				}

			}
		}
		m *= 2;
	}
}//table
 
int main()
{
	int k;			//n=2的k次方名选手
	printf("input k:");
	scanf("%d", &k);	
	int n = 1;
	int i, j;
	for (i = 1; i <= k; i++)
		n *= 2;

	int** res = (int**)malloc(sizeof(int*)*n);
	for (i = 0; i < n; i++)
		res[i] = (int*)malloc(sizeof(int)*n);

	table(k, n, res);
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
			printf("%d ", res[i][j]);
		printf("\n");
	}
	return 0;
}