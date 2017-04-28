#include<stdio.h>
#include<stdlib.h>


/*
k = 3, n =8
��ʼ	
1 2 3 4 5 6 7 8 
��һ�Σ�4�θ�ֵ��
1 2		3 4		5 6		7 8
2 1		4 3		6 5		8 7

12 34 56 78
21 43 65 87

�ڶ��Σ�2�θ�ֵ��
12		34		56		78
21		43		65		87
34		12		78		56
43		21		87		65

�����Σ�1�θ�ֵ��
56 78	12 34
65 87	21 43
78 56	34 12
87 54	43 21
*/

void table(int k, int n, int **a)	//ѭ�����ճ̱�
{
	int i, j, t, s;
	for (i = 0; i < n; i++)
		a[0][i] = i + 1;

	int m = 1;		//��¼�㶨��ǰm��	

	for (s = 0; s < k; s++)		//�ܹ���Ҫ����k�β���
	{
		n /= 2;
		for (t = 0; t < n; t++)	//ÿ�β�����ֵ�Ŀ���Ϊn
		{
			for (i = m ; i < 2 * m; i++)//������  
			{
				for (j = m; j < 2 * m; j++)//������  
				{
					a[i][j + t *m * 2] = a[i - m][j + t *m * 2 - m];//���½ǵ������Ͻǵ�ֵ  
					a[i][j + t *m * 2 - m] = a[i - m][j + t *m * 2];//���½ǵ������Ͻǵ�ֵ  
				}

			}
		}
		m *= 2;
	}
}//table
 
int main()
{
	int k;			//n=2��k�η���ѡ��
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