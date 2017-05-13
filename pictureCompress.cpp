#include<stdio.h>
#include<stdlib.h>

void Compress(int n, int p[], int s[], int l[], int b[]);	//压缩
//p: 像素点序列 s: 存储前i个像素点所需空间  l: 第i段像素点序列长度   b: 第i段像素点像素位数
//最后一段的段长度和像素位数分别存储在l[n]和b[n]中
//其前一段的段长度和像素位数存储于l[n-l[n]]和b[n-l[n]]中
int length(int n);
void Tracebace(int n, int &i, int s[], int l[]);			//用于输出结果
void Output(int s[], int l[], int b[], int n);				//输出结果

int main()
{
	int n;
	scanf("%d", &n);		//像素点个数

	int *p = (int*)malloc(sizeof(int)*(n + 1));
	int *s = (int*)malloc(sizeof(int)*(n + 1));
	int *l = (int*)malloc(sizeof(int)*(n + 1));
	int *b = (int*)malloc(sizeof(int)*(n + 1));
	for (int i = 1; i <= n; i++)
	{
		scanf("%d", &p[i]);
	}

	Compress(n, p, s, l, b);
	Output(s, l, b, n);

	free(p);
	p = NULL;
	free(s);
	s = NULL;
	free(l);
	l = NULL;
	free(b);
	b = NULL;

	return 0;

}

void Compress(int n, int p[], int s[], int l[], int b[])
{
	int Lmax = 256;
	int header = 11;
	s[0] = 0;
	int i, j, bmax;
	for (i = 1; i <= n; i++)	//像素序列规模逐渐增大
	{
		b[i] = length(p[i]);
		bmax = b[i];
		s[i] = s[i - 1] + bmax;	//未加header
		l[i] = 1;
		for (j = 2; j <= i && j <= Lmax; j++)	//从右往左设置断点
		{
			if (bmax < b[i - j + 1])
			{
				bmax = b[i - j + 1];
			}
			if (s[i] > s[i - j] + j*bmax)	//同样不加header进行判断
			{
				s[i] = s[i - j] + j*bmax;
				l[i] = j;
			}
		}
		s[i] += header;
	}
}//Compress

int length(int n)
{
	int len = 1;
	n >>= 1;
	while (n > 0)
	{
		len++;
		n >>= 1;
	}
	return len;
}//length

void Tracebace(int n, int &i, int s[], int l[])	//求出每一子问题包含的规模
{
	if (n == 0)
		return;
	Tracebace(n - l[n], i, s, l);
	s[i++] = n - l[n];
	
}//Tracebace

int Maxb(int *s, int *l, int *b, int j)	//s[j]段内需要的最大比特位
{
	int bmax = 0;
	int i;
	if (j == 1)
	{
		bmax = b[1];
		for (i = 2; i <= s[1]; i++)
		{
			if (bmax < b[i])
			{
				bmax = b[i];
			}
		}
	}
	else
	{
		bmax = b[s[j - 1] + 1];
		for (i = s[j - 1] + 2; i <= s[j]; i++)
		{
			if (bmax < b[i])
			{
				bmax = b[i];
			}
		}
	}
	return bmax;
}//Maxb

void Output(int s[], int l[], int b[], int n)
{
	//在输出s[n]存储位数后，s[]数组则被重新赋值，用来存储分段的位置  
	printf("total space:%dbits\n", s[n]);
	int m = 0;
	Tracebace(n, m, s, l);
	s[m] = n;
	
	int j;
	for (j = 1; j <= m; j++)
	{
		l[j] = l[s[j]];
		b[j] = Maxb(s, l, b, j);
		printf("length:%d\tbits:%d\n", l[j], b[j]);
	}
}//Output

