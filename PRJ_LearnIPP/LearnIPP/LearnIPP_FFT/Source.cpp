/*
// Copyright 2015 2017 Intel Corporation All Rights Reserved.
//
// The source code, information and material ("Material") contained herein is
// owned by Intel Corporation or its suppliers or licensors, and title
// to such Material remains with Intel Corporation or its suppliers or
// licensors. The Material contains proprietary information of Intel
// or its suppliers and licensors. The Material is protected by worldwide
// copyright laws and treaty provisions. No part of the Material may be used,
// copied, reproduced, modified, published, uploaded, posted, transmitted,
// distributed or disclosed in any way without Intel's prior express written
// permission. No license under any patent, copyright or other intellectual
// property rights in the Material is granted to or conferred upon you,
// either expressly, by implication, inducement, estoppel or otherwise.
// Any license under such intellectual property rights must be express and
// approved by Intel in writing.
//
// Unless otherwise agreed by Intel in writing,
// you may not remove or alter this notice or any other notice embedded in
// Materials by Intel or Intel's suppliers or licensors in any way.
*/

//   A simple example of performing the forward FFT for processing real data.
// implemented with Intel IPP functions:
//     ippiFFTGetSize_R_32f
//     ippiFFTInit_R_32f
//     ippiFFTFwd_RToPack_32f_C1R


#include <stdio.h>
#include "ipp.h"

/* Next two defines are created to simplify code reading and understanding */
#define EXIT_MAIN exitLine:                                  /* Label for Exit */
#define check_sts(st) if((st) != ippStsNoErr) goto exitLine; /* Go to Exit if IPP function returned status different from ippStsNoErr */

/* Results of ippMalloc() are not validated because IPP functions perform bad arguments check and will return an appropriate status  */

int main(void)
{
	IppStatus status;
	IppiFFTSpec_R_32f *pSpec = NULL;  /* Pointer to FFT spec structure */

									  /* Pointers to source/destination images */
	Ipp32f pSrcFwd[64] = { 0 }, pDstFwd[64] = { 0 };

	/* Pointer to the work buffers */
	Ipp8u *pMemInit = NULL;
	Ipp8u *pBuffer = NULL;

	int sizeSpec = 0, sizeInit = 0, sizeBuf = 0; /* size of FFT spec structure, Init and work buffers */

	pSrcFwd[0] = -3; pSrcFwd[9] = 1;

	check_sts(status = ippiFFTGetSize_R_32f(3, 3, IPP_FFT_DIV_INV_BY_N, ippAlgHintAccurate,
		&sizeSpec, &sizeInit, &sizeBuf))

		/* memory allocation */
	pSpec = (IppiFFTSpec_R_32f*)ippMalloc(sizeSpec);
	pBuffer = (Ipp8u*)ippMalloc(sizeBuf);
	pMemInit = (Ipp8u*)ippMalloc(sizeInit);

	/* inverse FFT transform */
	check_sts(status = ippiFFTInit_R_32f(3, 3, IPP_FFT_DIV_INV_BY_N, ippAlgHintAccurate, pSpec, pMemInit))

	check_sts(status = ippiFFTFwd_RToPack_32f_C1R(pSrcFwd, 8 * sizeof(Ipp32f),
			pDstFwd, 8 * sizeof(Ipp32f), pSpec, pBuffer))

	EXIT_MAIN
	ippFree(pMemInit);
	ippFree(pSpec);
	ippFree(pBuffer);
	printf("Exit status %d (%s)\n", (int)status, ippGetStatusString(status));
	return (int)status;
}
