#include <stdio.h>
#include <ipp.h>

int main(void)
{

	int sizeSpec = 0, sizeInit = 0, sizeBuf = 0;
	IppStatus status;
	
	status = ippiFFTGetSize_R_32f(3, 3, IPP_FFT_DIV_INV_BY_N, ippAlgHintAccurate, &sizeSpec, &sizeInit, &sizeBuf);

	IppiFFTSpec_R_32f* fftSpec;
	Ipp8u* memInit;
	fftSpec = (IppiFFTSpec_R_32f*)ippMalloc(sizeSpec);
	memInit = (Ipp8u*)ippMalloc(sizeInit);

	status = ippiFFTInit_R_32f(3, 3, IPP_FFT_DIV_INV_BY_N, ippAlgHintAccurate, fftSpec, memInit);

	
	Ipp8u* workBuffer = (Ipp8u*)ippMalloc(sizeBuf);


	Ipp32f src[8 * 8] = { 0 };
	ippiSet_32f_C1R(10, src, 8 * sizeof(Ipp32f), { 8, 8 });

	for (int i = 0; i < 64; i++)
	{
		printf("%f ", src[i]);
	}
	printf("\n");

	Ipp32f dst[8 * 8] = { 0 };
	status - ippiFFTFwd_RToPack_32f_C1R(src, 8 * sizeof(Ipp32f), dst, 8 * sizeof(Ipp32f), fftSpec, workBuffer);


	// Magnitude
	int magSize = 0;
	status = ippiMagnitudePackGetBufferSize_32f(1, { 8, 8 }, &magSize);
	Ipp8u* magBuffer = (Ipp8u*)ippMalloc(magSize);

	Ipp32f mag[8 * 8] = { 0 };

	status = ippiMagnitudePack_32f_C1R(dst, 8 * sizeof(Ipp32f), mag, 8 * sizeof(Ipp32f), {8, 8}, magBuffer);
	printf("mag:\n");
	for (int i = 0; i < 64; i++)
	{
		printf("%f ", mag[i]);
		if ((i+1) % 8 == 0)
		{
			printf("\n");
		}
	}
	printf("\n");

	
	// Inverse
	Ipp32f res[8 * 8] = { 0 };
	ippiFFTInv_PackToR_32f_C1R(dst, 8 * sizeof(Ipp32f), res,  8*sizeof(Ipp32f), fftSpec, workBuffer);

	for (int i = 0; i < 64; i++)
	{
		printf("%f\n", res[i]);
	}


	IppiSize roiSize = { 0 };


	ippFree(fftSpec);
	ippFree(memInit);
	ippFree(workBuffer);

	return 0;
}