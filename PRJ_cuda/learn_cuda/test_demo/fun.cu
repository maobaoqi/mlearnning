// kernel definition
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

__global__ void vecAdd(float* A, float* B, float* C)
{
	int idx = threadIdx.x;
	C[idx] = A[idx] + B[idx];
}

//int main(void)
//{
//
//	//vecAdd << <1, N >> > (A, B, C);
//}