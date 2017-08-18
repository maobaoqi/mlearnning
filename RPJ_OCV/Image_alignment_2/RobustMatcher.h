#pragma once

#include <opencv2\opencv.hpp>

class RobustMatcher
{
private:
	cv::Ptr<cv::Feature2D> detector;
	cv::Ptr<cv::Feature2D> computor;

	int normType;
	float ratio;
	bool refineF;
	double distance;
	double confidence;

public:
	RobustMatcher()
		: normType(cv::NORM_HAMMING),
		ratio(0.8f),
		refineF(true),
		distance(3.0),
		confidence(0.98)
	{
		detector = cv::ORB::create();
		computor = cv::BRISK::create();
	}

	
	~RobustMatcher()
	{

	}

	
	cv::Mat match
	(
		cv::Mat image1, cv::Mat image2,
		std::vector<cv::DMatch> &matches,
		std::vector<cv::KeyPoint> &keypoints1,
		std::vector<cv::KeyPoint> &keypoints2
	);

	
private:

	cv::Mat ransacTest
	(
		const std::vector<cv::DMatch> &matches,
		const std::vector<cv::KeyPoint> &keypoints1,
		const std::vector<cv::KeyPoint> &keypoints2,
		std::vector<cv::DMatch> &outMatches
	);

};
