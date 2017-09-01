#include "OpticalFlowDetectMotion.h"

#include <iostream>

#define PI 3.1415926

void OpticalFlowDetectMotion::SetReferenceImage(const cv::Mat & referenceImage)
{
	m_referenceImage = referenceImage;
	cv::GaussianBlur(m_referenceImage, m_referenceImage, cv::Size(5, 5), 1.5, 1.5);
	//cv::resize(m_referenceImage, m_referenceImage, cv::Size(), 0.5, 0.5);

	m_detector->detect(m_referenceImage, m_referenceKeyFeatures, cv::noArray());
	m_referencePoints.clear();
	for (auto keyFeature : m_referenceKeyFeatures)
	{
		m_referencePoints.push_back(keyFeature.pt);
	}

}

void OpticalFlowDetectMotion::CalMotion(const cv::Mat & queryImage, cv::Mat & transformMatrix)
{
	std::vector<cv::Point2f> queryImagePoints;
	std::vector<uchar> track_status;

	cv::Mat queryImageTemp;
	cv::GaussianBlur(queryImage, queryImageTemp, cv::Size(5, 5), 1.5, 1.5);
	//cv::resize(queryImageTemp, queryImageTemp, cv::Size(), 0.5, 0.5);
	
	cv::calcOpticalFlowPyrLK
	(
		m_referenceImage,
		queryImageTemp,
		m_referencePoints,
		queryImagePoints,
		track_status,
		cv::noArray(),
		cv::Size(2 * 15 + 1, 2 * 15 + 1),
		5,
		cv::TermCriteria(
			cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS,
			50,
			0.3
		)
	);

	std::vector<cv::Point2f> tmpRef, tmpQuery;
	for (int i = 0; i < (int)m_referencePoints.size(); i++)
	{
		if (!track_status[i])
		{
			continue;
		}
		tmpRef.push_back(m_referencePoints[i]);
		tmpQuery.push_back(queryImagePoints[i]);
	}

	std::vector<MotionInfo> allMotionInfo;
	MotionInfo singleMotionInfo;
	for (int i = 0; i < tmpRef.size(); i++)
	{
		float diffX = tmpRef[i].x - tmpQuery[i].x;
		float diffY = tmpRef[i].y - tmpQuery[i].y;
		singleMotionInfo.diffX = diffX;
		singleMotionInfo.diffY = diffY;

		float length = diffX*diffX + diffY*diffY;
		singleMotionInfo.length = sqrt(length);

		float angle;
		if (abs(diffX) < 1e-2)
		{
			if (abs(diffY) < 1e-2)
			{
				angle = 0.0f;
			}
			else if (diffY < -(1e-2))
			{
				angle = -(float)PI / 2;
			}
			else if (diffY > 1e-2)
			{
				angle = (float)PI / 2;
			}
		}
		else
		{
			angle = std::atanf(diffY / diffX);
		}
		singleMotionInfo.angle = angle;

		allMotionInfo.push_back(singleMotionInfo);
	}

	float step = -(float)PI / 2;
	std::vector<std::vector<MotionInfo>> motionHistogram;
	std::vector<MotionInfo> motionSet;

	while (allMotionInfo.size() > 0)
	{
		motionSet.clear();

		for (auto iter = allMotionInfo.cbegin(); iter != allMotionInfo.cend();)
		{
			if (iter->angle < step + 1e-2)
			{
				motionSet.push_back(*iter);
				iter = allMotionInfo.erase(iter);
			}
			else
			{
				iter++;
			}
		}

		motionHistogram.push_back(motionSet);
		step += (float)PI / 32;
	}

	int maxCountIndex = 0;
	int maxSize = 0;
	for (int i = 0; i < motionHistogram.size(); i++)
	{
		if (maxSize < motionHistogram[i].size())
		{
			maxCountIndex = i;
			maxSize = motionHistogram[i].size();
		}
	}
	motionSet = motionHistogram[maxCountIndex];

	std::swap(motionSet, allMotionInfo);

	motionHistogram.clear();
	step = 0.0f;
	while (allMotionInfo.size() > 0)
	{
		motionSet.clear();

		for (auto iter = allMotionInfo.cbegin(); iter != allMotionInfo.cend();)
		{
			if (iter->length < step + 1e-2)
			{
				motionSet.push_back(*iter);
				iter = allMotionInfo.erase(iter);
			}
			else
			{
				iter++;
			}
		}

		motionHistogram.push_back(motionSet);
		step += 3.0f;
	}

	maxCountIndex = 0;
	maxSize = 0;
	for (int i = 0; i < motionHistogram.size(); i++)
	{
		if (maxSize < motionHistogram[i].size())
		{
			maxCountIndex = i;
			maxSize = motionHistogram[i].size();
		}
	}
	motionSet = motionHistogram[maxCountIndex];


	float averageDX = 0, averageDY = 0;
	for (int i = 0; i < motionSet.size(); i++)
	{
		averageDX += motionSet[i].diffX;
		averageDY += motionSet[i].diffY;
	}
	averageDX /= motionSet.size();
	averageDY /= motionSet.size();

	// motion 
	std::cout << "Delta X:" << averageDX*2 << " Delta Y:" << averageDY*2 << std::endl;


	// transfrom
	cv::Mat transformMat(2, 3, CV_32F);
	transformMat.at<float>(0, 0) = 1.0f;
	transformMat.at<float>(0, 1) = 0.0f;
	transformMat.at<float>(0, 2) = averageDX;

	transformMat.at<float>(1, 0) = 0.0f;
	transformMat.at<float>(1, 1) = 1.0f;
	transformMat.at<float>(1, 2) = averageDY;

	transformMatrix = transformMat;
}
