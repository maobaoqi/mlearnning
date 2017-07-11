#include "ccfun_ipp.h"
#include <iostream>

#include "find_shift.h"

void PhaseCorrelation(Ipp8u* ref, Ipp8u* src, IppiSize roi, int &wshift, int &hshift)
{
	IppStatus status;

	int sizeSpec, sizeInit, sizeBuf;
	int orderX, orderY;
	orderX = 12;
	orderY = 12;
	status = ippiFFTGetSize_R_32f(orderX, orderY, IPP_FFT_DIV_INV_BY_N, IppHintAlgorithm::ippAlgHintAccurate, &sizeSpec, &sizeInit, &sizeBuf);

	IppiFFTSpec_R_32f* pFFTSpec = (IppiFFTSpec_R_32f*)ippMalloc(sizeSpec);
	Ipp8u* pMemInit = (Ipp8u*)ippMalloc(sizeInit);
	Ipp8u* ipp8uBuffer = (Ipp8u*)ippMalloc(sizeBuf);

	status = ippiFFTInit_R_32f(orderX, orderY,IPP_FFT_DIV_INV_BY_N, IppHintAlgorithm::ippAlgHintAccurate, pFFTSpec, pMemInit);

	// FFT
	int ipp32steps = roi.width * sizeof(Ipp32f);
	//Ipp32f* ref_tmp = ippiMalloc_32f_C1(roi.width, roi.height, &ipp32steps);
	//Ipp32f* src_tmp = ippiMalloc_32f_C1(roi.width, roi.height, &ipp32steps);
	Ipp32f* ref_tmp = new Ipp32f[roi.width * roi.height];
	Ipp32f* src_tmp = new Ipp32f[roi.width * roi.height];


	clock_t t1, t2;
	t1 = clock();
	ippiConvert_8u32f_C1R(ref, roi.width * sizeof(Ipp8u), ref_tmp, roi.width * sizeof(Ipp32f), roi);
	status = ippiFFTFwd_RToPack_32f_C1R(ref_tmp, roi.width * sizeof(Ipp32f), ref_tmp, roi.width * sizeof(Ipp32f), pFFTSpec, ipp8uBuffer);

	ippiConvert_8u32f_C1R(src, roi.width * sizeof(Ipp8u), src_tmp, roi.width * sizeof(Ipp32f), roi);
	status = ippiFFTFwd_RToPack_32f_C1R(src_tmp, roi.width * sizeof(Ipp32f), src_tmp, roi.width * sizeof(Ipp32f), pFFTSpec, ipp8uBuffer);

	// Get Magnitude
	//Ipp32f* ref_tmp_Mag = ippiMalloc_32f_C1(roi.width, roi.height, &ipp32steps);
	//Ipp32f* src_tmp_Mag = ippiMalloc_32f_C1(roi.width, roi.height, &ipp32steps);
	//
	//int magSize;
	//ippiMagnitudePackGetBufferSize_32f(1, roi, &magSize);
	//Ipp8u* magBuffer = (Ipp8u*)ippMalloc(magSize);
	//
	//status = ippiMagnitudePack_32f_C1R(ref_tmp, ipp32steps, ref_tmp_Mag, ipp32steps, roi, magBuffer);
	//status = ippiMagnitudePack_32f_C1R(src_tmp, ipp32steps, src_tmp_Mag, ipp32steps, roi, magBuffer);
	//
	//// Get Phase
	//Ipp32f* ref_tmp_Pha = ippiMalloc_32f_C1(roi.width, roi.height, &ipp32steps);
	//Ipp32f* src_tmp_Pha = ippiMalloc_32f_C1(roi.width, roi.height, &ipp32steps);
	//
	//int phaSize;
	//ippiPhasePackGetBufferSize_32f(1, roi, &phaSize);
	//Ipp8u* phaBuffer = (Ipp8u*)ippMalloc(phaSize);
	//
	//status = ippiPhasePack_32f_C1R(ref_tmp, ipp32steps, ref_tmp_Pha, ipp32steps, roi, phaBuffer);
	//status = ippiPhasePack_32f_C1R(src_tmp, ipp32steps, src_tmp_Pha, ipp32steps, roi, phaBuffer);

	// Cross-Correlation
	//ippiMulPackConj_32f_C1R(ref_tmp, ipp32steps, src_tmp, ipp32steps, src_tmp, ipp32steps, roi);
	ippiMulPackConj_32f_C1IR(ref_tmp, ipp32steps, src_tmp, ipp32steps, roi);

	// IFFT
	//ippiFFTInv_PackToR_32f_C1R(src_tmp, ipp32steps, src_tmp, ipp32steps, pFFTSpec, ipp8uBuffer);
	ippiFFTInv_PackToR_32f_C1IR(src_tmp, ipp32steps, pFFTSpec, ipp8uBuffer);

	// Find Location
	Ipp32f maxValue;
	int Xindx, Yindx;
	ippiMaxIndx_32f_C1R(src_tmp, ipp32steps, roi, &maxValue, &Xindx, &Yindx);

	LocationPair shift = { 0, 0 };
	FindShift(src_tmp, shift, roi.width, roi.height, {(float)Xindx, (float)Yindx});


	t2 = clock();

	std::cout << "IPP:" << double(t2 - t1) / CLOCKS_PER_SEC * 1000 << std::endl;

	delete[] ref_tmp;
	delete[] src_tmp;
	//ippiFree(ref_tmp);
	//ippiFree(src_tmp);

}