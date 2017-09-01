// A simple example of performing Optical Flow algorithm 
// using Intel IPP functions:
//     ippiPyramidGetSize
//     ippiPyramidInit
//     ippiPyramidLayerDownGetSize_8u_C1R
//     ippiPyramidLayerDownInit_8u_C1R
//     ippiPyramidLayerDown_8u_C1R
//     ippiOpticalFlowPyrLKGetSize
//     ippiOpticalFlowPyrLKInit_8u_C1R


#include <stdio.h>
#include "ipp.h"

#define WIDTH  640
#define HEIGHT 480


int main(void)
{
	IppStatus status = ippStsNoErr;
	Ipp8u* pPrevFrame = nullptr, *pNextFrame = nullptr;
	int prevStep = 0, nextStep = 0;         
	IppiSize roiSize = { WIDTH, HEIGHT };   
	int            numLevel = 5;            
	float          rate = 2.0f;            
	Ipp16s pKernel[5] = { 1,1,1,1,1 };
	int            kerSize = 5;             
	IppiPoint_32f *prevPt = nullptr;           
	IppiPoint_32f *nextPt = nullptr;           
	Ipp8s         *pStatus = nullptr;           
	Ipp32f        *pError = nullptr;          
	int            numFeat = 200;           
	int            winSize = 41;            
	int            numIter = 5;
	float          threshold = 0.0001f;

	Ipp8u* pBuf1 = nullptr, *pPBuf2 = nullptr, *pPBuf1 = nullptr;

	Ipp8u* pBuf2 = nullptr;
	Ipp8u* pStBuf1 = nullptr, *pStBuf2 = nullptr;
	Ipp8u* pOFStBuf = nullptr;

	pPrevFrame = ippiMalloc_8u_C1(roiSize.width, roiSize.height, &prevStep);
	pNextFrame = ippiMalloc_8u_C1(roiSize.width, roiSize.height, &nextStep);
	
	prevPt = (IppiPoint_32f*)ippsMalloc_8u(numFeat * 2 * sizeof(IppiPoint_32f));
	nextPt = (IppiPoint_32f*)ippsMalloc_8u(numFeat * 2 * sizeof(IppiPoint_32f));
	
	pStatus = (Ipp8s*)ippsMalloc_8u(numFeat);
	pError = (Ipp32f*)ippsMalloc_8u(numFeat * sizeof(Ipp32f));

	status = ippsSet_8u(0, (Ipp8u*)prevPt, numFeat * 2 * sizeof(IppiPoint_32f));
	status = ippsSet_8u(0, (Ipp8u*)nextPt, numFeat * 2 * sizeof(IppiPoint_32f));
	
	IppiPyramid *pPyr1 = nullptr, *pPyr2 = nullptr;
	IppiOptFlowPyrLK *pOF = nullptr;        
	int   PyrSize, BufSize, StateSize;

	status = ippiPyramidGetSize(&PyrSize, &BufSize, numLevel, roiSize, rate);

	pBuf1 = ippsMalloc_8u(BufSize);
	pPBuf1 = ippsMalloc_8u(PyrSize);

	/* Initialize structure for pyramids, calculates ROI for layers */
	status = ippiPyramidInit(&pPyr1, numLevel, roiSize, rate, pPBuf1, pBuf1);
	ippsFree(pBuf1); 
	pBuf1 = NULL;

	/* Compute the size of the structure for pyramids and the size of the required work buffer (in bytes) */
	status = ippiPyramidGetSize(&PyrSize, &BufSize, numLevel, roiSize, rate);

	pBuf1 = ippsMalloc_8u(BufSize);
	pPBuf2 = ippsMalloc_8u(PyrSize);
	/* Initialize structure for pyramids, calculates ROI for layers */
	status = ippiPyramidInit(&pPyr2, numLevel, roiSize, rate, pPBuf2, pBuf1);
	ippsFree(pBuf1); 
	pBuf1 = NULL;

	IppiPyramidDownState_8u_C1R **pState1 = (IppiPyramidDownState_8u_C1R**)&(pPyr1->pState);/* Pyramid state structure */
	IppiPyramidDownState_8u_C1R **pState2 = (IppiPyramidDownState_8u_C1R**)&(pPyr2->pState);/* Pyramid state structure */
	Ipp8u   **pImg1 = pPyr1->pImage; /* Pointer to the source image */
	Ipp8u   **pImg2 = pPyr2->pImage; /* Pointer to the source image */
	int     *pStep1 = pPyr1->pStep, *pStep2 = pPyr2->pStep;
	IppiSize *pRoi1 = pPyr1->pRoi, *pRoi2 = pPyr2->pRoi;
	IppHintAlgorithm hint = ippAlgHintFast;
	int i, level = pPyr1->level;
	int pStateSize1, pBufSize1, pStateSize2, pBufSize2;

	/* Calculate the size of structure for creating a lower(an upper) pyramid layer and the size of the temporary buffer */
	status = ippiPyramidLayerDownGetSize_8u_C1R(roiSize, rate, kerSize, &pStateSize1, &pBufSize1);

	pBuf1 = ippsMalloc_8u(pBufSize1);
	pStBuf1 = ippsMalloc_8u(pStateSize1);

	/* Initialize the structure for creating a lower(an upper) pyramid layer */
	status = ippiPyramidLayerDownInit_8u_C1R((IppiPyramidDownState_8u_C1R**)pState1, roiSize, rate, pKernel, kerSize, IPPI_INTER_LINEAR, pStBuf1, pBuf1);
	ippsFree(pBuf1); 
	pBuf1 = NULL;

	/* Calculate the size of structure for creating a lower(an upper) pyramid layer and the size of the temporary  buffer */
	status = ippiPyramidLayerDownGetSize_8u_C1R(roiSize, rate, kerSize, &pStateSize2, &pBufSize2);
	pBuf2 = ippsMalloc_8u(pBufSize2);
	pStBuf2 = ippsMalloc_8u(pStateSize2);

	/* Initialize the structure for creating a lower(an upper) pyramid layer */
	status = ippiPyramidLayerDownInit_8u_C1R((IppiPyramidDownState_8u_C1R**)pState2, roiSize, rate, pKernel, kerSize, IPPI_INTER_LINEAR, pStBuf2, pBuf2);
	ippsFree(pBuf2); 
	pBuf2 = NULL;

	pImg1[0] = (Ipp8u*)pPrevFrame;
	pImg2[0] = (Ipp8u*)pNextFrame;
	pStep1[0] = prevStep;
	pStep2[0] = nextStep;
	pRoi1[0] = pRoi2[0] = roiSize;
	for (i = 1; i <= level; i++)
	{
		pPyr1->pImage[i] = ippiMalloc_8u_C1(pRoi1[i].width, pRoi1[i].height,
			pStep1 + i);
		pPyr2->pImage[i] = ippiMalloc_8u_C1(pRoi2[i].width, pRoi2[i].height,
			pStep2 + i);
		/* Perform downsampling / upsampling of the image with 5x5 Gaussian kernel*/
		status = ippiPyramidLayerDown_8u_C1R(pImg1[i - 1], pStep1[i - 1], pRoi1[i - 1], pImg1[i], pStep1[i], pRoi1[i], *pState1);
		status = ippiPyramidLayerDown_8u_C1R(pImg2[i - 1], pStep2[i - 1], pRoi2[i - 1], pImg2[i], pStep2[i], pRoi2[i], *pState2);
	}

	status = ippiOpticalFlowPyrLKGetSize(winSize, roiSize, ipp8u, (IppHintAlgorithm)((int)hint), &StateSize);
	pOFStBuf = (Ipp8u*)ippsMalloc_8u(StateSize);

	/* Initialize a structure for pyramidal L-K algorithm */
	status = ippiOpticalFlowPyrLKInit_8u_C1R((IppiOptFlowPyrLK_8u_C1R**)&pOF, roiSize, winSize, (IppHintAlgorithm)((int)hint), pOFStBuf);

	/* Indicate an error condition if maxLev or threshold has negative value, or maxIter has zero or negative value */
	status = ippiOpticalFlowPyrLK_8u_C1R(pPyr1, pPyr2, prevPt, nextPt, pStatus, pError, numFeat, winSize, numLevel, numIter, threshold, pOF);

	for (i = level; i > 0; i--)
	{
		if (pImg2[i]) ippiFree(pImg2[i]);
		if (pImg1[i]) ippiFree(pImg1[i]);
	}
	

	ippsFree(pOFStBuf);
	ippsFree(pBuf1);
	ippsFree(pBuf2);
	ippsFree(pPBuf1);
	ippsFree(pPBuf2);
	ippsFree(pStBuf1);
	ippsFree(pStBuf2);
	ippiFree(pPrevFrame);
	ippiFree(pNextFrame);
	ippsFree(prevPt);
	ippsFree(nextPt);
	ippsFree(pStatus);
	ippsFree(pError);
	
	printf("Exit status %d (%s)\n", (int)status, ippGetStatusString(status));
    
	return (int)status;
}
