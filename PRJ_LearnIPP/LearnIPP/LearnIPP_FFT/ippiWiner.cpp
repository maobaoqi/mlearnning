


#include <stdio.h>
#include "ipp.h"

#define WIDTH  128  /* image width */
#define HEIGHT  64  /* image height */

int main(void)
{
	IppStatus status = ippStsNoErr;
	Ipp32f* pSrc = NULL, *pDst = NULL;    /* Pointers to source/destination images */
	Ipp32f* pSrcW = NULL;                 /* Pointers to source ROI image */
	int srcStep, dstStep;                 /* Steps, in bytes, through the source/destination images */
	IppiSize roiSize = { WIDTH, HEIGHT }; /* Size of source/destination in pixels */
	IppiSize maskSize = { 3, 3 };
	IppiSize dstRoiSize = { 0 };            /* Size of destination ROI in pixels */
	IppiPoint anchor = { 1, 1 };          /* Anchor cell specifying the mask alignment\
										  with respect to the position of the input pixel */
	Ipp32f noise[5] = { 1.0 };            /* Noise level value */
	Ipp8u *pBuffer = NULL;                /* Pointer to the work buffer */
	int iTmpBufSize = 0;                  /* Common work buffer size */
	int numChannels = 1;

	dstRoiSize.width = WIDTH - maskSize.width;
	dstRoiSize.height = HEIGHT - maskSize.height;

	/* memory allocation */
	pSrc = ippiMalloc_32f_C1(roiSize.width, roiSize.height, &srcStep);
	pDst = ippiMalloc_32f_C1(roiSize.width, roiSize.height, &dstStep);

	status = ippiImageJaehne_32f_C1R(pSrc, srcStep, roiSize);

	status = ippiFilterWienerGetBufferSize(dstRoiSize, maskSize, numChannels, &iTmpBufSize);

	status = ippiSet_32f_C1R(1.f, pDst, dstStep, roiSize);

	pBuffer = ippsMalloc_8u(iTmpBufSize);
	pSrcW = (Ipp32f*)((Ipp8u*)(pSrc + anchor.x) + anchor.y * srcStep);

	status = ippiFilterWiener_32f_C1R(pSrcW, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, noise, pBuffer);


		
	ippsFree(pBuffer);
	ippiFree(pSrc);
	ippiFree(pDst);
	
	printf("Exit status %d (%s)\n", (int)status, ippGetStatusString(status));
	return (int)status;
}
