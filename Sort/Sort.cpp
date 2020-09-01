// Sort.cpp : 优化版基数排序，冒泡排序，快速排序。
//

#include "stdafx.h"

//性能优化的基数排序
const static char radix = 64; //基
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


void Random(int *p, int n, int l, int r)//生成范围在l~r的随机数 
{
	srand(time(0));  //设置时间种子
	for (int i = 0; i < n; i++)
	{
		p[i] = rand() % (r - l + 1) + l;//生成区间r~l的随机数 
	}
}


/**
* @brief		返回对应位的数值
*
* @param data	原始数据
* @param i		对应位
* @return char	对应位的数值
*/
char GetPart(int data, char i) //返回对应位的数值
{
	return data >> offset[i] & (radix - 1);
}

/**
* @brief 	性能优化的基数排序，以2的指数幂为基，排序数组最大元素个数为200
*
* @param p 待排序的数组
* @param n 数组个数
*/
void RadixSort(int *p, int n) //基数排序，从小至大
{
	// int *bucket = (int *)malloc(sizeof(int) * n);
	int count[radix]; //计数排序用计数数组
	char i = 0;
	int j = 0;
	memset(bucket, 0, sizeof(bucket)); //bucket数组清零

	for (i = 0; i < 2; ++i)
	{
		memset(count, 0, sizeof(int) * radix); //计数数组清零

		for (j = 0; j < n; ++j)
		{
			count[GetPart(p[j], i)]++;
		}

		for (j = 1; j < radix; ++j)	//从小到大排列
		{
			count[j] += count[j - 1];
		}
		//for (j = radix - 1; j > 0; --j)	//从大到小排列
		//{
		//	count[j - 1] += count[j];
		//}

		for (j = n - 1; j >= 0; --j)
		{
			int k = GetPart(p[j], i);
			bucket[count[k] - 1] = p[j];
			count[k]--;
		}

		memcpy(p, bucket, sizeof(int) * n); //将排好序的数组复制回原数组
	}
	// free(bucket); //释放数组空间
}


void Sort(int*p, int amount)	//冒泡排序
{
	int i, j, tmp;
	for (j = 0; j < amount - 1; j++)	//冒泡排序
	{
		for (i = 0; i < (amount - 1 - j); i++)
		{
			if (p[i] > p[i + 1])	//从小到大排列
									//if (p[i] < p[i + 1])	//从大到小排列
			{
				tmp = p[i];
				p[i] = p[i + 1];
				p[i + 1] = tmp;
			}
		}
	}
}


void QuickSort(int arr[], const int left, const int right)	//快速排序
{
	if (left > right)
		return;
	int tmp = arr[left];	//哨兵
	int i = left;	//左索引值
	int j = right;	//右索引值

	while (i != j)
	{
		while (arr[j] >= tmp && j > i)	//从小到大排列>=
			j--;
		while (arr[i] <= tmp && j > i)	//从小到大排列<=
			i++;
		//while (arr[j] <= tmp && j > i)	//从大到小排列<=
		//	j--;
		//while (arr[i] >= tmp && j > i)	//从大到小排列>=
		//	i++;
		if (j > i)	//将比哨兵大的值放右侧，比哨兵小的值放左侧
		{
			int t = arr[i];
			arr[i] = arr[j];
			arr[j] = t;
		}
	}
	arr[left] = arr[i];	//原哨兵arr[left]更新新值
	arr[i] = tmp;	//原哨兵arr[left]归位，此时哨兵左边的值都比它小，右边的值都比它大

					//递归
	QuickSort(arr, left, i - 1);	//归位后的哨兵左侧数据快排
	QuickSort(arr, i + 1, right);	//归位后的哨兵右侧数据快排
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
	const int num = 200;	//待排序的数组元素个数
							//int br[num] = { 20, 80, 90, 589, 998, 965, 852, 123, 456, 789 };
	int br[num], br2[num], br3[num];

	//生成并复制随机数组
	Random(br, num, 0, 4095);	//生成随机数的通常范围为0~32767，这里通过取模控制取值为0~4095
	memcpy(br2, br, sizeof(br));
	memcpy(br3, br, sizeof(br));
	cout << "原数据如下：" << endl;
	PrintArr(br, num);

	//计时，精度：us
	QueryPerformanceFrequency(&litmp);//获得计时器的时钟频率
	dfFreq = (double)litmp.QuadPart;

	QueryPerformanceCounter(&litmp);
	startTime = litmp.QuadPart; //开始计时

	RadixSort(br, num);	//基数排序

	QueryPerformanceCounter(&litmp);
	endTime = litmp.QuadPart; //终止计时
	dfMinus = (double)(endTime - startTime);//计算计数器值
	dfTime[0] = dfMinus / dfFreq * 1000000;//获得对应时间，单位为秒 你可以乘1000000精确到微秒级（us）

	QueryPerformanceCounter(&litmp);
	startTime = litmp.QuadPart; //开始计时

	Sort(br2, num);	//冒泡排序

	QueryPerformanceCounter(&litmp);
	endTime = litmp.QuadPart; //终止计时
	dfMinus = (double)(endTime - startTime);//计算计数器值
	dfTime[1] = dfMinus / dfFreq * 1000000;//获得对应时间，单位为秒 你可以乘1000000精确到毫秒级（us）

	QueryPerformanceCounter(&litmp);
	startTime = litmp.QuadPart; //开始计时

	QuickSort(br3, num);	//快速排序

	QueryPerformanceCounter(&litmp);
	endTime = litmp.QuadPart; //终止计时
	dfMinus = (double)(endTime - startTime);//计算计数器值
	dfTime[2] = dfMinus / dfFreq * 1000000;//获得对应时间，单位为秒 你可以乘1000000精确到毫秒级（us）

										   //校验排序结果是否一致
	for (i = 0; i < num; i++)
	{
		if (((br3[i] - br2[i]) != 0) || ((br[i] - br2[i]) != 0))
		{
			isCorrect = 0;
			break;
		}
	}

	//输出排序结果及用时
	cout << "排序后数据如下：" << endl;
	PrintArr(br, num);
	cout << "基数排序用时：" << dfTime[0] << "us" << endl;
	cout << "冒泡排序用时：" << dfTime[1] << "us" << endl;
	cout << "快速排序用时：" << dfTime[2] << "us" << endl;
	cout << "排序结果一致？：（1-一致，0-不一致）" << isCorrect << endl;

	system("pause");
	return 0;
}

