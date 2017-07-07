#include <ippi.h>
#include <ipps.h>
#include <ippch.h>

#include <iostream>
#include <math.h>
#include <deque>

void HistogramExmaple()
{
	const int HEIGHT = 200;
	const int WIDTH = 200;
	Ipp8u pImg[HEIGHT * WIDTH];
	IppiSize roi = { WIDTH, HEIGHT };
	IppStatus sts;

	IppsRandUniState_8u* pRndObj;
	int sizeRndObj;
	ippsRandUniformGetSize_8u(&sizeRndObj);

	pRndObj = (IppsRandUniState_8u*)ippsMalloc_8u(sizeRndObj);

	// initialize rnd spec
	ippsRandUniformInit_8u(pRndObj, 0, 255, 0);

	// fill image
	for (int k = 0; k < HEIGHT; k++)
	{
		sts = ippsRandUniform_8u(pImg + k*WIDTH, WIDTH, pRndObj);
	}

	ippsFree(pRndObj);

	

	// Histogram
	const int nBins = 255;
	int nLevels[] = { nBins + 1 };
	Ipp32f lowerLevel[] = { 0 };
	Ipp32f upperLevel[] = { 255 };
	Ipp32f pLevels[nBins + 1], *ppLevels[1];
	int sizeHistObj, sizeBuffer;

	IppiHistogramSpec* pHistObj;
	Ipp8u* pBuffer;
	Ipp32u pHistVec[256];

	// get sizes for spec and buffer
	ippiHistogramGetBufferSize(ipp8u, roi, nLevels, 1, 1, &sizeHistObj, &sizeBuffer);

	pHistObj = (IppiHistogramSpec*)ippsMalloc_8u(sizeHistObj);
	pBuffer = (Ipp8u*)ippsMalloc_8u(sizeBuffer);

	// initialize spec
	ippiHistogramUniformInit(ipp8u, lowerLevel, upperLevel, nLevels, 1, pHistObj);

	// check levels of bins
	ppLevels[0] = pLevels;
	sts = ippiHistogramGetLevels(pHistObj, ppLevels);

	// calculate histogram
	sts = ippiHistogram_8u_C1R(pImg, WIDTH, roi, pHistVec, pHistObj, pBuffer);

	for (int i = 0; i < 256; i++)
	{
		std::cout << "The pixel gray sacale >" << i << ": " << pHistVec[i] << std::endl;
	}

	ippsFree(pHistObj);
	ippsFree(pBuffer);

}


int main(void)
{
	//HistogramExmaple();
	//int order;
	//int length = 1026;
	//order = int(ceil(log2(length)));
	//
	//std::cout << order << std::endl;

	std::deque<int> thisDeque;
	int i = 0;
	while (i < 10)
	{
		thisDeque.push_front(i);
		i++;
	}

	int value = thisDeque.front();
	std::cout << value << std::endl;

	return 0;
}