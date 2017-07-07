/*
* Copyright (C) 2016 Direct Electron Incorporated
* All rights reserved.
*
* File: gpuFunc.cu
* Description:
*
*  Implementation of all the functions using gpu cuda.
*
* Author: Shaobai Li
* Date: 13/10/2016
*/

#include "gpuFunc.h"

#include "stdio.h"
using namespace GPUSpace;


__global__ void cuAdd(float *dst, float *src, int size)
{
	int id = blockIdx.x*blockDim.x + threadIdx.x;
	if (id >= size) return;
	dst[id] += src[id];
}
__global__ void cuMulNum(float *dst, float num, int size)
{
	int id = blockIdx.x*blockDim.x + threadIdx.x;
	if (id >= size) return;
	dst[id] *= num;
}

void GPUSpace::GPUAdd(float *dst, float *src, int size)
{
	cuAdd << <size / BLOCKSIZE + 1, BLOCKSIZE >> >(dst, src, size);
}

void GPUSpace::GPUMulNum(float *dst, float num, int size)
{
	cuMulNum << <size / BLOCKSIZE + 1, BLOCKSIZE >> >(dst, num, size);
}

bool GPUSpace::GPUFFT2d(float* fourierBuf, cufftHandle plan)
{
	cufftResult r = cufftExecR2C(plan,
		(cufftReal*)fourierBuf,
		(cufftComplex *)fourierBuf);
	if (r != CUFFT_SUCCESS)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool GPUSpace::GPUIFFT2d(float* fourierBuf, cufftHandle plan)
{
	cufftResult r = cufftExecC2R(plan,
		(cufftComplex *)fourierBuf,
		(cufftReal*)fourierBuf);
	if (r != CUFFT_SUCCESS)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void GPUSpace::GPUCopyROI(float * dst, int dM, int dx, int dy,
	float * src, int sM, int sx, int sy,
	int roiX, int roiY)
{
	/*
	* dst, src        pointer of destination/source image
	* dM, sM          width of destination/source image
	* dx, dy, sx, sy  offset x/y of destination/source image
	* roiX, roiY      width/height of region of interest
	*/

	int j;
	for (j = 0; j < roiY; ++j)
	{
		cudaMemcpy(dst + ((j + dy)*dM + dx), src + ((j + sy)*sM + sx),
			sizeof(float)*roiX, cudaMemcpyDeviceToDevice);
	}
}

bool GPUSpace::GPUSimpleCrop(float *dst, float *src, int nColsDst, int nColsSrc,
	int offsetX, int offsetY, int nColsROI, int nRowsROI)
{
	/*
	if(src == 0 || dst == 0) return false;
	if(nsamin == 0 || nsamout == 0) return false;
	if((offset+nsamout) > nsamin) return false;*/

	//int nsaminb=nsamin.width() + 2;
	//int nsamoutb=nsamout.width() + 2;

	GPUSpace::GPUCopyROI(dst, nColsDst, 0, 0,
		src, nColsSrc, offsetX, offsetY,
		nColsROI, nRowsROI);

	return true;
}

bool GPUSpace::GPUFFTBin(float *dst, float *src,
	int nColsDst, int nColsSrc, int nRowsDst, int nRowsSrc)
{


	int i;
	int size = sizeof(float)*(nColsDst);
	for (i = 0; i<nRowsDst / 2; i++)
	{
		//up half
		if (cudaMemcpy(dst + i*(nColsDst), src + i*(nColsSrc), size, cudaMemcpyDeviceToDevice) != cudaSuccess) return false;

		//down half
		if (cudaMemcpy(dst + (nRowsDst - 1 - i)*(nColsDst), src + (nRowsSrc - 1 - i)*(nColsSrc), size, cudaMemcpyDeviceToDevice) != cudaSuccess) return false;
	}

	return true;
}



static __device__ cuComplex conj(cuComplex f)
{
	f.y *= -1.0;
	return f;
}

static __device__ cuComplex cXc(cuComplex a, cuComplex b) // a*b
{
	cuComplex c;
	c.x = a.x*b.x - a.y*b.y;
	c.y = a.x*b.y + a.y*b.x;
	return c;
}
static __device__ float cabs(cuComplex a) // a*b
{
	return sqrt(a.x*a.x + a.y*a.y);
}

__global__ void cuFFTLogModulus(float *dMod, cuComplex *dfft, int size, float scale)
{
	int id = blockIdx.x*blockDim.x + threadIdx.x;
	if (id >= size) return;
	dMod[id] = log(1 + cabs(dfft[id])*scale);
}

void GPUSpace::GPUFFTLogModulus(float *dMod, float *dfft, int nCols, int nRows, float scale)
{
	int size = (nCols / 2 + 1)*nRows;
	cuFFTLogModulus << <size / BLOCKSIZE + 1, BLOCKSIZE >> >(dMod, (cuComplex *)dfft, size, scale);
}



__global__ void cuShift(cuComplex *dBuf, Pixel *dPosList,
	float shX, float shY, int size)
{
	int id = blockIdx.x*blockDim.x + threadIdx.x;
	if (id >= size) return;

	float shift = shX*dPosList[id].x + shY*dPosList[id].y;
	cuComplex phase;
	phase.x = cos(shift);
	phase.y = sin(shift);

	dBuf[id] = cXc(dBuf[id], phase);

}

void GPUSpace::GPUFFTShift(float * dBuf, Pixel * dPosList,
	float shX, float shY, int width, int height)
{

	float shx = shX*2.0F*3.1415926F / width;
	float shy = shY*2.0F*3.1415926F / height;

	int size = (width / 2 + 1)*height;

	cuShift << <size / BLOCKSIZE + 1, BLOCKSIZE >> >((cuComplex *)dBuf,
		dPosList, shx, shy, size);
}

__global__ void cuFourierCC(cuComplex *dPar0BufFB, cuComplex *dPar1BufFB, Pixel *dPosList,
	int size)
{
	int id = blockIdx.x*blockDim.x + threadIdx.x;
	if (id >= size) return;

	Pixel pos = dPosList[id];

	cuComplex phase;
	phase.x = 1;
	phase.y = 0;

	cuComplex val = cXc(dPar1BufFB[id], conj(cXc(dPar0BufFB[id], phase)));
	val.x /= size;
	val.x *= pos.val;
	val.y /= size;
	val.y *= pos.val;
	dPar0BufFB[id] = val;

}

void GPUSpace::GPUFourierCC(float *dPar0BufFB, float *dPar1BufFB, Pixel *dPosList,
	int halfWidthBF, int height)
{


	int size = halfWidthBF * height;
	cuFourierCC << <size / BLOCKSIZE + 1, BLOCKSIZE >> >((cuComplex *)dPar0BufFB,
		(cuComplex *)dPar1BufFB,
		dPosList, size);

}


bool GPUSpace::GPUSync()
{
	if (cudaThreadSynchronize() != cudaSuccess)
	{
		printf("sync failed!!\n");
		return false;
	}
	if (cudaGetLastError() != cudaSuccess)
	{
		printf("something failed!!\n");
		return false;
	}
	return true;
}















