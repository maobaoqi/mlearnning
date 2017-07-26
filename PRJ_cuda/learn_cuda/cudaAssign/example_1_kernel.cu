#ifndef EXAMPLE_1_KERNEL_H_
#define EXAMPLE_1_KERNEL_H_

#include <cuda_device_runtime_api.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <cuda_runtime_api.h>
#include <cuda_occupancy.h>
#include <device_types.h>
#include <device_functions.h>
#include <device_atomic_functions.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <curt>

__global__ void testKernel(float* g_data_in, float* g_data_out)
{
	extern __shared__ float sdata[];

	const unsigned int blockID = blockIdx.x;
	const unsigned int threadID_in_block = threadIdx.x;
	const unsigned int threadID_in_grid = blockDim.x * blockIdx.x + threadIdx.x;

	sdata[threadID_in_block] = g_data_in[threadID_in_grid];
	__syncthreads();
}


// 简单的规约求和函数
__global__ static void reduceKernel(float* d_result, float* d_input, int N)
{
	const int threadID = blockDim.x * blockIdx.x + threadIdx.x;
	const int threadN = gridDim.x * blockDim.x;

	float sum = 0;
	for (int pos = threadID; pos < N; pos += threadN)
	{
		sum += d_input[pos];
	}

	d_result[threadID] = sum;
}

// 将内核函数封装成C函数
extern "C"
void lanunch_reduceKernel(float* d_result, float* d_input, int N, int BLOCK_N, int THREAD_N)
{
	reduceKernel <<<BLOCK_N, THREAD_N >>> (d_result, d_input, N);
}

#endif