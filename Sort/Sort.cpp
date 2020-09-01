// Sort.cpp : �Ż����������ð�����򣬿�������
//

#include "stdafx.h"

//�����Ż��Ļ�������
const static char radix = 64; //��
static char offset[2] = { 0, 6 };
int bucket[200];


void  PrintArr(int arr[], int n)
{
	for (int i = 0; i < n; ++i)
	{
		cout.width(4);
		cout << arr[i];
		if ((i + 1) % 15 == 0)
			cout << endl;
		else
			cout << " ";
	}
	cout << endl;
}


void Random(int *p, int n, int l, int r)//���ɷ�Χ��l~r������� 
{
	srand(time(0));  //����ʱ������
	for (int i = 0; i < n; i++)
	{
		p[i] = rand() % (r - l + 1) + l;//��������r~l������� 
	}
}


/**
* @brief		���ض�Ӧλ����ֵ
*
* @param data	ԭʼ����
* @param i		��Ӧλ
* @return char	��Ӧλ����ֵ
*/
char GetPart(int data, char i) //���ض�Ӧλ����ֵ
{
	return data >> offset[i] & (radix - 1);
}

/**
* @brief 	�����Ż��Ļ���������2��ָ����Ϊ���������������Ԫ�ظ���Ϊ200
*
* @param p �����������
* @param n �������
*/
void RadixSort(int *p, int n) //�������򣬴�С����
{
	// int *bucket = (int *)malloc(sizeof(int) * n);
	int count[radix]; //���������ü�������
	char i = 0;
	int j = 0;
	memset(bucket, 0, sizeof(bucket)); //bucket��������

	for (i = 0; i < 2; ++i)
	{
		memset(count, 0, sizeof(int) * radix); //������������

		for (j = 0; j < n; ++j)
		{
			count[GetPart(p[j], i)]++;
		}

		for (j = 1; j < radix; ++j)	//��С��������
		{
			count[j] += count[j - 1];
		}
		//for (j = radix - 1; j > 0; --j)	//�Ӵ�С����
		//{
		//	count[j - 1] += count[j];
		//}

		for (j = n - 1; j >= 0; --j)
		{
			int k = GetPart(p[j], i);
			bucket[count[k] - 1] = p[j];
			count[k]--;
		}

		memcpy(p, bucket, sizeof(int) * n); //���ź�������鸴�ƻ�ԭ����
	}
	// free(bucket); //�ͷ�����ռ�
}


void Sort(int*p, int amount)	//ð������
{
	int i, j, tmp;
	for (j = 0; j < amount - 1; j++)	//ð������
	{
		for (i = 0; i < (amount - 1 - j); i++)
		{
			if (p[i] > p[i + 1])	//��С��������
									//if (p[i] < p[i + 1])	//�Ӵ�С����
			{
				tmp = p[i];
				p[i] = p[i + 1];
				p[i + 1] = tmp;
			}
		}
	}
}


void QuickSort(int arr[], const int left, const int right)	//��������
{
	if (left > right)
		return;
	int tmp = arr[left];	//�ڱ�
	int i = left;	//������ֵ
	int j = right;	//������ֵ

	while (i != j)
	{
		while (arr[j] >= tmp && j > i)	//��С��������>=
			j--;
		while (arr[i] <= tmp && j > i)	//��С��������<=
			i++;
		//while (arr[j] <= tmp && j > i)	//�Ӵ�С����<=
		//	j--;
		//while (arr[i] >= tmp && j > i)	//�Ӵ�С����>=
		//	i++;
		if (j > i)	//�����ڱ����ֵ���Ҳ࣬���ڱ�С��ֵ�����
		{
			int t = arr[i];
			arr[i] = arr[j];
			arr[j] = t;
		}
	}
	arr[left] = arr[i];	//ԭ�ڱ�arr[left]������ֵ
	arr[i] = tmp;	//ԭ�ڱ�arr[left]��λ����ʱ�ڱ���ߵ�ֵ������С���ұߵ�ֵ��������

					//�ݹ�
	QuickSort(arr, left, i - 1);	//��λ����ڱ�������ݿ���
	QuickSort(arr, i + 1, right);	//��λ����ڱ��Ҳ����ݿ���
}

void QuickSort(int arr[], int n)
{
	QuickSort(arr, 0, n - 1);
}


int main()
{
	LARGE_INTEGER litmp;
	LONGLONG startTime, endTime;
	double dfMinus, dfFreq, dfTime[3];

	int i = 0;
	bool isCorrect = 1;
	const int num = 200;	//�����������Ԫ�ظ���
							//int br[num] = { 20, 80, 90, 589, 998, 965, 852, 123, 456, 789 };
	int br[num], br2[num], br3[num];

	//���ɲ������������
	Random(br, num, 0, 4095);	//�����������ͨ����ΧΪ0~32767������ͨ��ȡģ����ȡֵΪ0~4095
	memcpy(br2, br, sizeof(br));
	memcpy(br3, br, sizeof(br));
	cout << "ԭ�������£�" << endl;
	PrintArr(br, num);

	//��ʱ�����ȣ�us
	QueryPerformanceFrequency(&litmp);//��ü�ʱ����ʱ��Ƶ��
	dfFreq = (double)litmp.QuadPart;

	QueryPerformanceCounter(&litmp);
	startTime = litmp.QuadPart; //��ʼ��ʱ

	RadixSort(br, num);	//��������

	QueryPerformanceCounter(&litmp);
	endTime = litmp.QuadPart; //��ֹ��ʱ
	dfMinus = (double)(endTime - startTime);//���������ֵ
	dfTime[0] = dfMinus / dfFreq * 1000000;//��ö�Ӧʱ�䣬��λΪ�� ����Գ�1000000��ȷ��΢�뼶��us��

	QueryPerformanceCounter(&litmp);
	startTime = litmp.QuadPart; //��ʼ��ʱ

	Sort(br2, num);	//ð������

	QueryPerformanceCounter(&litmp);
	endTime = litmp.QuadPart; //��ֹ��ʱ
	dfMinus = (double)(endTime - startTime);//���������ֵ
	dfTime[1] = dfMinus / dfFreq * 1000000;//��ö�Ӧʱ�䣬��λΪ�� ����Գ�1000000��ȷ�����뼶��us��

	QueryPerformanceCounter(&litmp);
	startTime = litmp.QuadPart; //��ʼ��ʱ

	QuickSort(br3, num);	//��������

	QueryPerformanceCounter(&litmp);
	endTime = litmp.QuadPart; //��ֹ��ʱ
	dfMinus = (double)(endTime - startTime);//���������ֵ
	dfTime[2] = dfMinus / dfFreq * 1000000;//��ö�Ӧʱ�䣬��λΪ�� ����Գ�1000000��ȷ�����뼶��us��

										   //У���������Ƿ�һ��
	for (i = 0; i < num; i++)
	{
		if (((br3[i] - br2[i]) != 0) || ((br[i] - br2[i]) != 0))
		{
			isCorrect = 0;
			break;
		}
	}

	//�������������ʱ
	cout << "������������£�" << endl;
	PrintArr(br, num);
	cout << "����������ʱ��" << dfTime[0] << "us" << endl;
	cout << "ð��������ʱ��" << dfTime[1] << "us" << endl;
	cout << "����������ʱ��" << dfTime[2] << "us" << endl;
	cout << "������һ�£�����1-һ�£�0-��һ�£�" << isCorrect << endl;

	system("pause");
	return 0;
}

