#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <cuda.h>
#include <cuda_runtime_api.h>
#include <cuda_device_runtime_api.h>
#include <IntShCut.h>

void runTest(int argc, char** agrv);

int main(int argc, char** argv)
{
	runTest(argc, argv);
	
	return 0;
}

void runTest(int argc, char** argv)
{
	// start
	cudaSetDevice(0);

	unsigned int num_blocks = 4;
	unsigned int num_threads = 4;

	unsigned int mem_size = sizeof(float)*num_threads*num_blocks;
	float* h_data_in = (float*)malloc(mem_size);
	float* h_data_out = (float*)malloc(mem_size);

	float* d_data_in;
	cudaMalloc((void**)&d_data_in, mem_size);
	float* d_data_out;
	cudaMalloc((void**)&d_data_out, mem_size);

	dim3 grid(num_blocks, 1, 1);
	dim3 threads(num_threads, 1, 1);

	// run kernel

	// call back
	cudaMemcpy(h_data_out, d_data_out, mem_size, cudaMemcpyDeviceToHost);

	for (unsigned int i = 0; i < num_blocks; i++)
	{
		for (unsigned int j = 0; j < num_threads; j++)
		{
			printf("%5.0f", h_data_out[i*num_threads + j]);
		}
		printf("\n");
	}

	free(h_data_in);
	free(h_data_out);
	cudaFree(d_data_in);
	cudaFree(d_data_out);
}