#include "find_shift.h"

#include <iostream>

void FindShift(float* ccMap, LocationPair & shift, int width, int height, LocationPair roughShift)
{

	shift.x = roughShift.x;
	shift.y = roughShift.y;

	//Fourier interpolation
	int lenSubBox = 16;
	Ipp32f* mapSubBox = new Ipp32f[lenSubBox * lenSubBox];
	int mapSubBox_step = lenSubBox * sizeof(Ipp32f);
	
	int mapSubBox_LTx = int(shift.x - lenSubBox / 2);
	int mapSubBox_LTy = int(shift.y - lenSubBox / 2);

	mapSubBox_LTx = (mapSubBox_LTx + lenSubBox) > width ? (width - lenSubBox) : (mapSubBox_LTx < 0 ? 0 : mapSubBox_LTx);
	mapSubBox_LTy = (mapSubBox_LTy + lenSubBox) > height ? (height - lenSubBox) : (mapSubBox_LTy < 0 ? 0 : mapSubBox_LTy);


	Ipp32f* srcHeader = ccMap + mapSubBox_LTy * width * sizeof(Ipp32f) + mapSubBox_LTx * sizeof(Ipp32f);
	ippiCopy_32f_C1R(srcHeader, width * sizeof(Ipp32f), mapSubBox, mapSubBox_step, { lenSubBox, lenSubBox });
	
	//RaisedCosine((cuComplex *)hMapSubBoxFB, lenSubBox);

	// Fourier tansform
	IppStatus status;
	int orderX = 4;
	int orderY = 4;
	int sizeSpec, sizeInit, sizeBuf;
	status = ippiFFTGetSize_R_32f(orderX, orderY, IPP_FFT_DIV_INV_BY_N, ippAlgHintAccurate, &sizeSpec, &sizeInit, &sizeBuf);
	IppiFFTSpec_R_32f* fftSpec = (IppiFFTSpec_R_32f*)ippMalloc(sizeSpec);
	Ipp8u* memInit = (Ipp8u*)ippMalloc(sizeInit);
	status = ippiFFTInit_R_32f(orderX, orderY, IPP_FFT_DIV_INV_BY_N, ippAlgHintAccurate, fftSpec, memInit);
	Ipp8u* buffer = (Ipp8u*)ippMalloc(sizeBuf);
	status = ippiFFTFwd_RToPack_32f_C1R(mapSubBox, mapSubBox_step, mapSubBox, mapSubBox_step, fftSpec, buffer);

	// Enlarge fourier specturm
	int scale = 32;
	int lenPad = lenSubBox * scale;
	
	Ipp32f* hFFTMapPadFB = new Ipp32f[lenPad * lenPad ];
	int step = lenPad * sizeof(Ipp32f);

	ippiSet_32f_C1R(0.0f, hFFTMapPadFB, step, { lenPad, lenPad });
	
	ippiCopy_32f_C1R(mapSubBox, 
		mapSubBox_step,
		hFFTMapPadFB, 
		step, 
		{ lenSubBox / 2, lenSubBox / 2 });

	ippiCopy_32f_C1R(mapSubBox + mapSubBox_step / 2, 
		mapSubBox_step,
		hFFTMapPadFB + (lenPad - lenSubBox / 2)*sizeof(Ipp32f), 
		step, 
		{ lenSubBox / 2, lenSubBox / 2 });

	ippiCopy_32f_C1R(mapSubBox + (lenSubBox / 2)*mapSubBox_step, 
		mapSubBox_step,
		hFFTMapPadFB + (lenPad - lenSubBox / 2)*step, 
		step, 
		{ lenSubBox / 2, lenSubBox / 2 });

	ippiCopy_32f_C1R(mapSubBox + (lenSubBox * (lenSubBox / 2) + (lenSubBox / 2))*sizeof(Ipp32f), 
		mapSubBox_step,
		hFFTMapPadFB + ((lenPad - lenSubBox / 2)*lenPad + (lenPad - lenSubBox / 2))*sizeof(Ipp32f), 
		step, 
		{ lenSubBox / 2, lenSubBox / 2 });

	// Inverse Fourier Transform
	int orderX2 = 9;
	int orderY2 = 9;
	int sizeSpec2, sizeInit2, sizeBuf2;
	status = ippiFFTGetSize_R_32f(orderX2, orderY2, IPP_FFT_DIV_INV_BY_N, ippAlgHintAccurate, &sizeSpec2, &sizeInit2, &sizeBuf2);
	IppiFFTSpec_R_32f* fftSpec2 = (IppiFFTSpec_R_32f*)ippMalloc(sizeSpec2);
	Ipp8u* memInit2 = (Ipp8u*)ippMalloc(sizeInit2);
	status = ippiFFTInit_R_32f(orderX2, orderY2, IPP_FFT_DIV_INV_BY_N, ippAlgHintAccurate, fftSpec2, memInit2);
	Ipp8u* buffer2 = (Ipp8u*)ippMalloc(sizeBuf2);
	status = ippiFFTInv_PackToR_32f_C1R(hFFTMapPadFB, step, hFFTMapPadFB, step, fftSpec2, buffer2);

	Ipp32f maxValue;
	int Xindx, Yindx;
	ippiMaxIndx_32f_C1R(hFFTMapPadFB, step, {lenPad, lenPad}, &maxValue, &Xindx, &Yindx);


	shift.x = mapSubBox_LTx + Xindx / double(scale);
	shift.y = mapSubBox_LTy + Yindx / double(scale);

	std::cout << shift.x << std::endl;
	std::cout << shift.y << std::endl;

	
	delete[] hFFTMapPadFB;
	delete[] mapSubBox;

	ippFree(fftSpec);
	ippFree(memInit);
	ippFree(buffer);

	ippFree(fftSpec2);
	ippFree(memInit2);
	ippFree(buffer2);
}