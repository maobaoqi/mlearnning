#pragma once
#include <opencv2\core.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\optflow.hpp>

#include <vector>

class OpticalFlowDetectMotion
{
public:
	OpticalFlowDetectMotion()
	{
		m_detector = cv::ORB::create();
		m_descriptor = cv::BRISK::create();
	}


	~OpticalFlowDetectMotion()
	{

	}

	
	void SetReferenceImage(const cv::Mat & referenceImage);


	void CalMotion(const cv::Mat & queryImage, cv::Mat & transformMatrix);


protected:
	cv::Mat m_referenceImage;

	std::vector<cv::Point2f> m_referencePoints;
	std::vector<cv::KeyPoint> m_referenceKeyFeatures;

	cv::Ptr<cv::Feature2D> m_detector;
	cv::Ptr<cv::Feature2D> m_descriptor;

	struct MotionInfo
	{
		float diffX;
		float diffY;
		float length;
		float angle;
	};
};