#include "ccfun_ipp.h"
#include <iostream>

#include "find_shift.h"

void PhaseCorrelation(Ipp8u* ref, Ipp8u* src, IppiSize roi, int &wshift, int &hshift)
{
	IppStatus status;

	int sizeSpec, sizeInit, sizeBuf;
	int orderX, orderY;
	orderX = 13;
	orderY = 13;
	status = ippiFFTGetSize_R_32f(orderX, orderY, IPP_FFT_DIV_FWD_BY_N, IppHintAlgorithm::ippAlgHintAccurate, &sizeSpec, &sizeInit, &sizeBuf);

	IppiFFTSpec_R_32f* pFFTSpec = (IppiFFTSpec_R_32f*)ippMalloc(sizeSpec);
	Ipp8u* pMemInit = (Ipp8u*)ippMalloc(sizeInit);
	Ipp8u* ipp8uBuffer = (Ipp8u*)ippMalloc(sizeBuf);

	status = ippiFFTInit_R_32f(orderX, orderY, IPP_FFT_DIV_FWD_BY_N, IppHintAlgorithm::ippAlgHintAccurate, pFFTSpec, pMemInit);

	// FFT
	int ipp32steps = roi.width * sizeof(Ipp32f)*2;
	Ipp32f* ref_tmp = new Ipp32f[roi.width * roi.height * 4];
	Ipp32f* src_tmp = new Ipp32f[roi.width * roi.height * 4];


	clock_t t1, t2;
	t1 = clock();

	ippiSet_32f_C1R(0.0f, ref_tmp, ipp32steps, { roi.width*2, roi.height*2});
	ippiConvert_8u32f_C1R(ref, roi.width * sizeof(Ipp8u), ref_tmp, ipp32steps, roi);

	status = ippiFFTFwd_RToPack_32f_C1R(ref_tmp, ipp32steps, ref_tmp, ipp32steps, pFFTSpec, ipp8uBuffer);

	ippiSet_32f_C1R(0.0f, src_tmp, ipp32steps, { roi.width*2, roi.height*2});
	ippiConvert_8u32f_C1R(src, roi.width * sizeof(Ipp8u), src_tmp, ipp32steps, roi);
	status = ippiFFTFwd_RToPack_32f_C1R(src_tmp, ipp32steps, src_tmp, ipp32steps, pFFTSpec, ipp8uBuffer);

	// Cross-Correlation
	ippiMulPackConj_32f_C1IR(ref_tmp, ipp32steps, src_tmp, ipp32steps, {roi.width*2, roi.height*2});

	// rcpack2d to complex
	Ipp32fc* tmp32fc = new Ipp32fc[roi.width*roi.height * 4];
	ippiPackToCplxExtend_32f32fc_C1R(src_tmp, { roi.width * 2, roi.height * 2 }, ipp32steps, tmp32fc, roi.width * 2 * sizeof(Ipp32fc));

	// magnitude
	int sizeMag;
	ippiMagnitudePackGetBufferSize_32f(1, { roi.width * 2, roi.height * 2 }, &sizeMag);
	Ipp8u* magBuffer = (Ipp8u*)ippMalloc(sizeMag);

	Ipp32f *magImg = new Ipp32f[roi.width * roi.height * 4];
	ippiMagnitudePack_32f_C1R(src_tmp, ipp32steps, magImg, ipp32steps, { roi.width * 2, roi.height * 2 }, magBuffer);

	// .....
	Ipp32fc* pfcNumber(tmp32fc);
	Ipp32f* pfNumber(magImg);
	for (int i = 0; i < roi.height * roi.width * 4; i++)
	{
		const Ipp32f ctmp(*pfNumber);
		if (0.f < ctmp)
		{
			pfcNumber->re /= ctmp;
			pfcNumber->im /= ctmp;
		}
		++pfcNumber;
		++pfNumber;
	}

	// complex to rcpack2d
	ippiCplxExtendToPack_32fc32f_C1R(tmp32fc, roi.width * 2 * sizeof(Ipp32fc), { roi.width * 2, roi.height * 2 }, src_tmp, ipp32steps);

	// IFFT
	ippiFFTInv_PackToR_32f_C1IR(src_tmp, ipp32steps, pFFTSpec, ipp8uBuffer);
	
	// Find Location
	Ipp32f maxValue;
	int Xindx, Yindx;
	ippiMaxIndx_32f_C1R(src_tmp, ipp32steps, { roi.width*2, roi.height*2}, &maxValue, &Xindx, &Yindx);

	Xindx = Xindx > (roi.width*2 / 2) ? (Xindx - roi.width*2) : Xindx;
	Yindx = Yindx > (roi.height*2 / 2) ? (Yindx - roi.height*2) : Yindx;

	LocationPair shift = { 0, 0 };
	//FindShift(src_tmp, shift, roi.width, roi.height, {(float)Xindx, (float)Yindx});


	t2 = clock();

	std::cout << "IPP:" << double(t2 - t1) / CLOCKS_PER_SEC * 1000 << std::endl;

	delete[] ref_tmp;
	delete[] src_tmp;
	delete[] magImg;
	//delete[] imTEP;
	delete[] tmp32fc;
	//delete[] tmp2;
	//ippiFree(ref_tmp);
	//ippiFree(src_tmp);

}