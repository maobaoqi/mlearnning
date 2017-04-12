#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2\core.hpp>

using namespace cv;

void Test(int n)
{
	for (int i = 0; i < 10000; i++)
	{
		for (int j = 0; j < 10000; j++)
		{
			// do nothing
		}
	}
	printf("%d, ", n);
}

void main(void)
{
	double t = (double)getTickCount();

	for (int i = 0; i < 10; i++)
	{
		Test(i);
	}
	t = ((double)getTickCount() - t) / getTickFrequency();
	printf("Time without OpenMP: %.3fs\n", t);

	t = (double)getTickCount();

#pragma omp parallel for
	for (int i = 0; i < 10; i++)
	{
		Test(i);
	}

	t = ((double)getTickCount() - t) / getTickFrequency();
	printf("Time with OpenMP: %.3fs\n", t);
}