#include <ipp.h>
#include <iostream>


int main(int argc, char** argv)
{
	// get the sizes for required buffers
	int sizeSpec, sizeInit, sizeBuffer;
	int orderX, orderY;
	ippiFFTGetSize_R_32f(orderX, orderY, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone, &sizeSpec, &sizeInit, &sizeBuffer);

	// allocate memory for required buffers
	IppiFFTSpec_R_32f* pMemSpec = (IppiFFTSpec_R_32f*)ippMalloc(sizeSpec);

	Ipp8u* pInit = nullptr;
	if (sizeInit > 0)
	{
		pInit = (Ipp8u*)ippMalloc(sizeInit);
	}

	Ipp8u* pBuffer = nullptr;
	if (sizeBuffer > 0)
	{
		pBuffer = (Ipp8u*)ippMalloc(sizeBuffer);
	}

	// Initialize FFT specification structure
	ippiFFTInit_R_32f(orderX, orderY, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone, pMemSpec, pInit);

	// Free initialize buffer
	if (sizeInit > 0)
	{
		ippFree(pInit);
		pInit = nullptr;
	}
	if (sizeBuffer > 0)
	{
		ippFree(pBuffer);
		pInit = nullptr;
	}
	ippFree(pMemSpec);

	return 0;
}