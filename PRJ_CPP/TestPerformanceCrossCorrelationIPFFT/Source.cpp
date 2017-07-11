#include <ippi.h>
#include <fftw3.h>
#include <iostream>
#include <ctime>

#include "ccfun.h"
#include "ccfun_ipp.h"

int main()
{
	int w = 4096;
	int h = 4096;

	IppiSize roi = { w, h };
	int stepSize = w * sizeof(Ipp8u);

	//Ipp8u* refImg = ippiMalloc_8u_C1(w, h, &stepSize);
	//Ipp8u* srcImg = ippiMalloc_8u_C1(w, h, &stepSize);

	Ipp8u* refImg = new Ipp8u[w * h];
	Ipp8u* srcImg = new Ipp8u[w*h];

	
	ippiSet_8u_C1R(0, refImg, stepSize, roi);
	refImg[18 + 4096 * 105] = 30;
	refImg[19 + 4096 * 105] = 30;
	refImg[20 + 4096 * 105] = 30;
	//refImg[1003 + 2048 * 105] = 10;
	//refImg[1004 + 2048 * 105] = 10;
	//refImg[1005 + 2048 * 105] = 10;
	//refImg[1006 + 2048 * 105] = 20;
	//refImg[1007 + 2048 * 105] = 20;
	//refImg[1008 + 2048 * 105] = 20;
	//refImg[1009 + 2048 * 105] = 10;
	//refImg[1010 + 2048 * 105] = 10;
	//refImg[1011 + 2048 * 105] = 10;
	//refImg[1012 + 2048 * 105] = 5;
	//refImg[1013 + 2048 * 105] = 5;
	//refImg[1014 + 2048 * 105] = 5;


	
	ippiSet_8u_C1R(0, srcImg, stepSize, roi);
	srcImg[15 + 4096 * 500] = 30;
	srcImg[16 + 4096 * 500] = 30;
	srcImg[17 + 4096 * 500] = 30;
	
	
	int hshift = 0;
	int wshift = 0;

	//clock_t tStart, tEnd;
	
	//tStart = clock();
	//PhaseCorrelation2D(srcImg, refImg, hshift, wshift);
	//tEnd = clock();

	//std::cout << double(tEnd - tStart) / CLOCKS_PER_SEC * 1000 << std::endl;

	//tStart = clock();

	PhaseCorrelation(srcImg, refImg, roi, wshift, hshift);

    //tEnd = clock();

	//std::cout << double(tEnd - tStart) / CLOCKS_PER_SEC * 1000 << std::endl;
	
	//ippiFree(refImg);
	//ippiFree(srcImg);
	delete[] refImg;
	delete[] srcImg;

	return 0;
}