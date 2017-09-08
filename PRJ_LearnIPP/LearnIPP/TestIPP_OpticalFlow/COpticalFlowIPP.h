#pragma once
#ifndef COPTICAL_FLOW_IPP_H
#define COPTICAL_FLOW_IPP_H

// opencv header
#include <opencv2\core.hpp>

// ipp header
#include <ippi.h>
#include <ipp.h>

class COpticalFlowIPP
{
public:

	COpticalFlowIPP()
	{

	}

	
	~COpticalFlowIPP()
	{

	}


	void Initialize()
	{
		IppStatus status;
		status = ippStsNoErr;

		Ipp8u *pStBuf2 = nullptr;
		Ipp8u *pOFStBuf = nullptr;

		int width, height;
		IppiSize roiSize = { width, height };

		// create image
		Ipp8u* pPrevFrame = nullptr, *pNextFrame = nullptr;
		int prevStep = 0, nextStep = 0;
		pPrevFrame = ippiMalloc_8u_C1(roiSize.width, roiSize.height, &prevStep);
		pNextFrame = ippiMalloc_8u_C1(roiSize.width, roiSize.height, &nextStep);

		// create keypoints
		int numFeatures = 200;
		IppiPoint_32f *prevPt = nullptr;
		IppiPoint_32f *nextPt = nullptr;
		prevPt = (IppiPoint_32f*)ippsMalloc_8u(numFeatures * 2 * sizeof(IppiPoint_32f));
		nextPt = (IppiPoint_32f*)ippsMalloc_8u(numFeatures * 2 * sizeof(IppiPoint_32f));
		status = ippsSet_8u(0, (Ipp8u*)prevPt, numFeatures * 2 * sizeof(IppiPoint_32f));
		status = ippsSet_8u(0, (Ipp8u*)nextPt, numFeatures * 2 * sizeof(IppiPoint_32f));

		// set value
		int PyrSize, BufSize;
		int numLevel = 5;
		float rate = 1.5f;
		
		// create pyramid structure for image 1
		status = ippiPyramidGetSize(&PyrSize, &BufSize, numLevel, roiSize, rate);
		Ipp8u *pBuf1 = nullptr;
		pBuf1 = ippsMalloc_8u(BufSize);
		Ipp8u *pPBuf1 = nullptr;
		pPBuf1 = ippsMalloc_8u(PyrSize);

		IppiPyramid *pPyr1 = nullptr;
		status = ippiPyramidInit(&pPyr1, numLevel, roiSize, rate, pPBuf1, pBuf1);
		ippsFree(pBuf1);
		pBuf1 = nullptr;

		// create pyramid structure for image 2
		status = ippiPyramidGetSize(&PyrSize, &BufSize, numLevel, roiSize, rate);
		pBuf1 = ippsMalloc_8u(BufSize);
		Ipp8u* pPBuf2;
		pPBuf2 = ippsMalloc_8u(PyrSize);
		
		IppiPyramid *pPyr2 = nullptr;
		status = ippiPyramidInit(&pPyr2, numLevel, roiSize, rate, pPBuf2, pBuf1);
		ippsFree(pBuf1);
		pBuf1 = nullptr;

		// Initialize pyramid layer down
		IppiPyramidDownState_8u_C1R **pState1 = (IppiPyramidDownState_8u_C1R**)&(pPyr1->pState);
		IppiPyramidDownState_8u_C1R **pState2 = (IppiPyramidDownState_8u_C1R**)&(pPyr2->pState);
		int pStateSize1, pBufSize1, pStateSize2, pBufSize2;

		int kerSize = 5;// gaussian blur kernel
		status = ippiPyramidLayerDownGetSize_8u_C1R(roiSize, rate, kerSize, &pStateSize1, &pBufSize1);

		pBuf1 = ippsMalloc_8u(pBufSize1);
		Ipp8u *pStBuf1 = nullptr;
		pStBuf1 = ippsMalloc_8u(pStateSize1);

		Ipp16s pKernel[5] = { 1,1,1,1,1 };
		status = ippiPyramidLayerDownInit_8u_C1R((IppiPyramidDownState_8u_C1R**)pState1, roiSize, rate, pKernel, kerSize, IPPI_INTER_LINEAR, pStBuf1, pBuf1);
		ippsFree(pBuf1);
		pBuf1 = nullptr;

		status = ippiPyramidLayerDownGetSize_8u_C1R(roiSize, rate, kerSize, &pStateSize2, &pBufSize2);
		Ipp8u* pBuf2 = nullptr;
		pBuf2 = ippsMalloc_8u(pBufSize2);
		pStBuf2 = ippsMalloc_8u(pStateSize2);

		status = ippiPyramidLayerDownInit_8u_C1R((IppiPyramidDownState_8u_C1R**)pState2, roiSize, rate, pKernel, kerSize, IPPI_INTER_LINEAR, pStBuf2, pBuf2);
		ippsFree(pBuf2);
		pBuf2 = nullptr;

		// set value
		Ipp8u **pImg1 = pPyr1->pImage;
		Ipp8u **pImg2 = pPyr2->pImage;
		pImg1[0] = (Ipp8u*)pPrevFrame;
		pImg2[0] = (Ipp8u*)pNextFrame;

		int *pStep1 = pPyr1->pStep;
		int *pStep2 = pPyr2->pStep;
		pStep1[0] = prevStep;
		pStep2[0] = nextStep;
		IppiSize *pRoi1 = pPyr1->pRoi;
		IppiSize *pRoi2 = pPyr2->pRoi;
		pRoi1[0] = roiSize;
		pRoi2[0] = roiSize;

		int level = pPyr1->level;
		for (int i = 1; i <= level; i++)
		{
			pPyr1->pImage[i] = ippiMalloc_8u_C1(pRoi1[i].width, pRoi1[i].height, pStep1 + i);
			pPyr2->pImage[i] = ippiMalloc_8u_C1(pRoi2[i].width, pRoi2[i].height, pStep2 + i);
			
			status = ippiPyramidLayerDown_8u_C1R(pImg1[i - 1], pStep1[i - 1], pRoi1[i - 1], pImg1[i], pStep1[i], pRoi1[i], *pState1);
			status = ippiPyramidLayerDown_8u_C1R(pImg2[i - 1], pStep2[i - 1], pRoi2[i - 1], pImg2[i], pStep2[i], pRoi2[i], *pState2);
		}

		int winSize = 41;
		int StateSize;
		status = ippiOpticalFlowPyrLKGetSize(winSize, roiSize, ipp8u, IppHintAlgorithm::ippAlgHintFast, &StateSize);
		pOFStBuf = (Ipp8u*)ippsMalloc_8u(StateSize);

		IppiOptFlowPyrLK *pOF = nullptr;
		int numIter = 5;
		status = ippiOpticalFlowPyrLKInit_8u_C1R((IppiOptFlowPyrLK_8u_C1R**)&pOF, roiSize, winSize, IppHintAlgorithm::ippAlgHintFast, pOFStBuf);
		Ipp32f threshold = 0.0001f;
		Ipp8s *pStatus = nullptr;
		Ipp32f *pError = nullptr;
		pStatus = (Ipp8s*)ippsMalloc_8u(numFeatures);
		pError = (Ipp32f*)ippsMalloc_8u(numFeatures * sizeof(Ipp32f));
		status = ippiOpticalFlowPyrLK_8u_C1R(pPyr1, pPyr2, prevPt, nextPt, pStatus, pError, numFeatures, winSize, numLevel, numIter, threshold, pOF);


		// free memory
		for (int i = level; i > 0; i--)
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

		return;
	}


	void CalcMotion(const cv::Mat& src, const cv::Mat& ref)
	{

	}

};




#endif
