#pragma once
/*
* Copyright (C) 2016 Direct Electron Incorporated
* All rights reserved.
*
* File: gpuFunc.h
* Description:
*
*  Declaration of all the functions using gpu cuda.
*
* Author: Shaobai Li
* Date: 13/10/2016
*/
#include "cuda.h"
#include <cufft.h>
#include <cuda_runtime.h>

namespace GPUSpace
{
	struct DEPair
	{
		float x;
		float y;
	};


	struct Pixel
	{
		float val;
		float x;
		float y;
	};

	const int BLOCKSIZE = 1024;
	const float FFTSCALE = 0.0001F;

	bool GPUFFT2d(float* fourierBuf, cufftHandle plan);
	bool GPUIFFT2d(float* fourierBuf, cufftHandle plan);

	void GPUAdd(float *dst, float *src, int size);
	void GPUMulNum(float *dst, float num, int size);


	bool GPUSimpleCrop(float *dst, float *src, int nColsDst, int nColsSrc,
		int offsetX, int offsetY, int nColsROI, int nRowsROI);
	void GPUFFTLogModulus(float *dMod, float *dfft, int nCols, int nRows,
		float scale);
	bool GPUFFTBin(float *dst, float *src,
		int nColsDst, int nColsSrc, int nRowsDst, int nRowsSrc);

	void GPUFFTShift(float * dsumcorr, Pixel * dPosList,
		float shX, float shY, int width, int height);

	void GPUFourierCC(float *dPar0BufFB, float *dPar1BufFB, Pixel *dPosList,
		int halfWidthBF, int height);

	bool GPUSync();


	void GPUCopyROI(float * dst, int dM, int dx, int dy,
		float * src, int sM, int sx, int sy,
		int roiX, int roiY);
}