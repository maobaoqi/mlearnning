#include <ipp.h>
#include <ippi.h>

#include <opencv2\core.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\video.hpp>

#include <assert.h>
#include <vector>
#include <iostream>
#include <ctime>

int main(int argc, char** argv)
{
	// read test image from OpenCV API
	cv::Mat imgSrc = cv::imread("D:\\Test Images\\data2_stick\\test (1).tif", cv::IMREAD_GRAYSCALE);
	cv::Mat imgRef = cv::imread("D:\\Test Images\\data2_stick\\test (5).tif", cv::IMREAD_GRAYSCALE);

	// get keypoints using ORB
	cv::Ptr<cv::Feature2D> featuresDetector;
	featuresDetector = cv::ORB::create();

	std::vector<cv::KeyPoint> keypointsSrc, keypointsRef;
	//featuresDetector->detect(imgSrc, keypointsSrc, cv::noArray());
	featuresDetector->detect(imgRef, keypointsRef, cv::noArray());

	std::vector<cv::Point2f> kpRef, kpSrc;
	for (int i = 0; i < keypointsRef.size(); i++)
	{
		kpRef.push_back(keypointsRef.at(i).pt);
	}

	std::vector<uchar> OFstatus;

	clock_t opT1, opT2;
	opT1 = clock();
	cv::calcOpticalFlowPyrLK
	(
		imgRef,
		imgSrc,
		kpRef,
		kpSrc,
		OFstatus,
		cv::noArray(),
		cv::Size(31, 31),
		5,
		cv::TermCriteria(
			cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS,
			30,
			0.3
		)
	);
	opT2 = clock();

	std::cout << "Opencv Optical Flow: " << (double)(opT2 - opT1) / CLOCKS_PER_SEC * 1000 << std::endl;

	cv::Mat refTemp = imgRef.clone();
	std::vector<cv::Point2f> points1, points2;
	for (int i = 0; i < kpRef.size(); i++)
	{
		if (!OFstatus[i])
		{
			continue;
		}

		cv::line(refTemp, kpRef[i], kpSrc[i], cv::Scalar(255, 0, 0), 2, 8);
	}
	cv::resize(refTemp, refTemp, cv::Size(), 0.5, 0.5);


	IppStatus status;
	status = ippStsNoErr;


	IppiSize roiSize = { imgRef.cols, imgRef.rows };

	// create image && assign Mat pointer to IPP pointer
	Ipp8u* pSrcFrame = nullptr, *pRefFrame = nullptr;
	pSrcFrame = (Ipp8u*)imgSrc.data;
	pRefFrame = (Ipp8u*)imgRef.data;

	int srcStep = 0, refStep = 0;
	srcStep = imgSrc.cols * sizeof(Ipp8u);
	refStep = imgRef.cols * sizeof(Ipp8u);

	// create keypoints
	int numFeaturesRef = keypointsRef.size();
	int numFeaturesSrc = keypointsSrc.size();
	//int numFeatures = numFeaturesRef > numFeaturesSrc ? numFeaturesSrc : numFeaturesRef;
	int numFeatures = numFeaturesRef;
	IppiPoint_32f *refPt = nullptr;
	IppiPoint_32f *srcPt = nullptr;
	//refPt = (IppiPoint_32f*)ippsMalloc_8u(numFeatures * 2 * sizeof(IppiPoint_32f));
	//srcPt = (IppiPoint_32f*)ippsMalloc_8u(numFeatures * 2 * sizeof(IppiPoint_32f));
	
	refPt = (IppiPoint_32f*)new Ipp32f[numFeatures * 2 * sizeof(IppiPoint_32f)];
	srcPt = (IppiPoint_32f*)new Ipp32f[numFeatures * 2 * sizeof(IppiPoint_32f)];
	
	//ippsSet_8u(0, (Ipp8u*)srcPt, numFeatures * 2 * sizeof(IppiPoint_32f));
	
	for (int i = 0; i < numFeatures; i++)
	{
		refPt[i].x = keypointsRef[i].pt.x;
		refPt[i].y = keypointsRef[i].pt.y;

		srcPt[i].x = keypointsRef[i].pt.x;
		srcPt[i].y = keypointsRef[i].pt.y;
	}

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
	Ipp8u *pStBuf2 = nullptr;
	pStBuf2 = ippsMalloc_8u(pStateSize2);

	status = ippiPyramidLayerDownInit_8u_C1R((IppiPyramidDownState_8u_C1R**)pState2, roiSize, rate, pKernel, kerSize, IPPI_INTER_LINEAR, pStBuf2, pBuf2);
	ippsFree(pBuf2);
	pBuf2 = nullptr;

	// set value
	Ipp8u **pImg1 = pPyr1->pImage;
	Ipp8u **pImg2 = pPyr2->pImage;
	pImg1[0] = (Ipp8u*)pRefFrame;
	pImg2[0] = (Ipp8u*)pSrcFrame;

	int *pStep1 = pPyr1->pStep;
	int *pStep2 = pPyr2->pStep;
	pStep1[0] = refStep;
	pStep2[0] = srcStep;
	IppiSize *pRoi1 = pPyr1->pRoi;
	IppiSize *pRoi2 = pPyr2->pRoi;
	pRoi1[0] = roiSize;
	pRoi2[0] = roiSize;

	clock_t t1, t2;
	int level = pPyr1->level;
	for (int i = 1; i <= level; i++)
	{
		pPyr1->pImage[i] = ippiMalloc_8u_C1(pRoi1[i].width, pRoi1[i].height, pStep1 + i);
		pPyr2->pImage[i] = ippiMalloc_8u_C1(pRoi2[i].width, pRoi2[i].height, pStep2 + i);

		status = ippiPyramidLayerDown_8u_C1R(pImg1[i - 1], pStep1[i - 1], pRoi1[i - 1], pImg1[i], pStep1[i], pRoi1[i], *pState1);
		status = ippiPyramidLayerDown_8u_C1R(pImg2[i - 1], pStep2[i - 1], pRoi2[i - 1], pImg2[i], pStep2[i], pRoi2[i], *pState2);
	}

	int winSize = 31;
	int StateSize;
	status = ippiOpticalFlowPyrLKGetSize(winSize, roiSize, ipp8u, IppHintAlgorithm::ippAlgHintFast, &StateSize);
	Ipp8u *pOFStBuf = nullptr;
	pOFStBuf = (Ipp8u*)ippsMalloc_8u(StateSize);

	IppiOptFlowPyrLK *pOF = nullptr;
	int numIter = 30;
	status = ippiOpticalFlowPyrLKInit_8u_C1R((IppiOptFlowPyrLK_8u_C1R**)&pOF, roiSize, winSize, IppHintAlgorithm::ippAlgHintFast, pOFStBuf);
	Ipp32f threshold = 0.3f;
	Ipp8s *pStatus = nullptr;
	Ipp32f *pError = nullptr;
	//pStatus = (Ipp8s*)ippsMalloc_8u(numFeatures);
	pStatus = (Ipp8s*)new Ipp8s[numFeatures];
	//pError = (Ipp32f*)ippsMalloc_8u(numFeatures * sizeof(Ipp32f));
	pError = (Ipp32f*)new Ipp32f[numFeatures];

	t1 = clock(); // time start

	status = ippiOpticalFlowPyrLK_8u_C1R(pPyr1, pPyr2, refPt, srcPt, pStatus, pError, numFeatures, winSize, numLevel, numIter, threshold, pOF);

	t2 = clock(); // time end
	std::cout << "IPP Optical Flow: " << (double)(t2 - t1) / CLOCKS_PER_SEC * 1000 << std::endl;

	int count = 0;
	std::vector<cv::Point2f> kptsSrc, kptsRef;
	for (int i = 0; i < numFeatures; i++)
	{
		//float value = (float)pError[i];
		//std::cout << value << " ";
		//if (value)
		//{
		//	count++;
		//	kptsRef.push_back(keypointsRef[i]);
		//	kptsSrc.push_back(keypointsSrc[i]);
		//}
		count++;
		//kptsRef.push_back(keypointsRef[i]);
		cv::Point point;
		point.x = srcPt[i].x;
		point.y = srcPt[i].y;
		kptsSrc.push_back(point);
		
		point.x = refPt[i].x;
		point.y = refPt[i].y;
		kptsRef.push_back(point);
		//if ((i + 1) % 50 == 0)
		//{
		//	std::cout << std::endl;
		//}
	}
	std::cout << "none zero: " << count <<std::endl;

	// show result
	for (int i = 0; i < kptsSrc.size(); i++)
	{
		cv::line(imgRef, kptsRef[i], kptsSrc[i], cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
	}

	cv::resize(imgRef, imgRef, cv::Size(), 0.5, 0.5);
	cv::imshow("IPP REF", imgRef);

	cv::imshow("OpenCV REF", refTemp);

	cv::waitKey(0);

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
	//ippsFree(refPt);
	//ippsFree(srcPt);
	delete[] refPt;
	delete[] srcPt;
	//ippsFree(pStatus);
	//ippsFree(pError);
	delete[] pStatus;
	delete[] pError;

	pRefFrame = nullptr;
	pSrcFrame = nullptr;


	return 0;
}