#include<stdio.h>
#include<stdlib.h>

void Compress(int n, int p[], int s[], int l[], int b[]);	//ѹ��
//p: ���ص����� s: �洢ǰi�����ص�����ռ�  l: ��i�����ص����г���   b: ��i�����ص�����λ��
//���һ�εĶγ��Ⱥ�����λ���ֱ�洢��l[n]��b[n]��
//��ǰһ�εĶγ��Ⱥ�����λ���洢��l[n-l[n]]��b[n-l[n]]��
int length(int n);
void Tracebace(int n, int &i, int s[], int l[]);			//����������
void Output(int s[], int l[], int b[], int n);				//������

int main()
{
	int n;
	scanf("%d", &n);		//���ص����

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
	for (i = 1; i <= n; i++)	//�������й�ģ������
	{
		b[i] = length(p[i]);
		bmax = b[i];
		s[i] = s[i - 1] + bmax;	//δ��header
		l[i] = 1;
		for (j = 2; j <= i && j <= Lmax; j++)	//�����������öϵ�
		{
			if (bmax < b[i - j + 1])
			{
				bmax = b[i - j + 1];
			}
			if (s[i] > s[i - j] + j*bmax)	//ͬ������header�����ж�
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

void Tracebace(int n, int &i, int s[], int l[])	//���ÿһ����������Ĺ�ģ
{
	if (n == 0)
		return;
	Tracebace(n - l[n], i, s, l);
	s[i++] = n - l[n];
	
}//Tracebace

int Maxb(int *s, int *l, int *b, int j)	//s[j]������Ҫ��������λ
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
	//�����s[n]�洢λ����s[]���������¸�ֵ�������洢�ֶε�λ��  
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

