#include <stdio.h>
#include<string.h>
#include<stdlib.h>

void LCS(char *x, int xl, char *y, int yl, int **c, int **b);
//最长公共子串    xl: x's length   c: record the length of longest CS b: break point 

int main()
{
	char x[100], y[100];
	int xlength, ylength;
	int **c, **b;
	int i, j, k;

	printf("input x:");
	gets_s(x);
	xlength = strlen(x);

	printf("input y:");
	gets_s(y);
	ylength = strlen(y);

	b = (int**)malloc(sizeof(int*)*(xlength + 1));
	c = (int**)malloc(sizeof(int*)*(xlength + 1));

	for (i = 0; i <= xlength; i++)
	{
		b[i] = (int*)malloc(sizeof(int)*(ylength + 1));
		c[i] = (int*)malloc(sizeof(int)*(ylength + 1));
	}

	LCS(x, xlength, y, ylength, c, b);

	printf("%d\n", c[xlength][ylength]);

	i = xlength;
	j = ylength;
	for (k = 0; k < c[xlength][ylength];)		//ÄæÐòÊä³ö
	{
		if (1 == b[i][j])
		{
			printf("%c", x[i-1]);		
			i--; 
			j--;
			k++;
		}
		else if (2 == b[i][j])
		{
			i--;
		}
		else
		{
			j--;
		}
	}

	for (i = 0; i <= xlength; i++)
	{
		free(b[i]);
		free(c[i]);
	}

	free(b);
	b = NULL;
	free(c);
	c = NULL;

	return 0;
}

void LCS(char *x, int xl, char *y, int yl, int **c, int **b)
{
	int i, j;

	for (i = 0; i <= xl; i++)
	{
		c[i][0] = 0;
	}

	for (i = 1; i <= yl; i++)
	{
		c[0][i] = 0;
	}

	for (i = 1; i <= xl; i++)
	{
		for (j = 1; j <= yl; j++)
		{
			if (x[i-1] == y[j-1])
			{
				c[i][j] = c[i - 1][j - 1] + 1;
				b[i][j] = 1;
			}
			else if (c[i - 1][j] >= c[i][j - 1])
			{
				c[i][j] = c[i - 1][j];
				b[i][j] = 2;
			}
			else
			{
				c[i][j] = c[i][j-1];
				b[i][j] = 3;
			}
		}
	}
}//LCS
