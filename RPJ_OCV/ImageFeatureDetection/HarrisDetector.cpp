#include "HarrisDetector.h"
#include <opencv2\imgproc.hpp>


HarrisDetector::HarrisDetector()
{
}


HarrisDetector::~HarrisDetector()
{
}

void HarrisDetector::detect(const cv::Mat & image)
{
	cv::cornerHarris(image, cornerStrength, neighbourhood, aperture, k);
	cv::minMaxLoc(cornerStrength, nullptr, &maxStrength);

	cv::Mat dilated;
	cv::dilate(cornerStrength, dilated, cv::Mat());
	cv::compare(cornerStrength, dilated, localMax, cv::CMP_EQ);
}

void HarrisDetector::getCorners(std::vector<cv::Point>& points, double qualityLevel)
{
	cv::Mat cornerMap = getCornersMap(qualityLevel);
	getCornersPoints(points, cornerMap);
}

cv::Mat HarrisDetector::getCornersMap(double qualityLevel)
{
	cv::Mat cornerMap;
	threshold = qualityLevel * maxStrength;
	cv::threshold(cornerStrength, cornerTh, threshold, 255, cv::THRESH_BINARY);
	cornerTh.convertTo(cornerMap, CV_8U);
	cv::bitwise_and(cornerMap, localMax, cornerMap);

	return cornerMap;
}

void HarrisDetector::getCornersPoints(std::vector<cv::Point>& points, const cv::Mat & cornerMap)
{
	for (int y = 0; y < cornerMap.rows; y++)
	{
		const uchar* rowPtr = cornerMap.ptr<uchar>(y);
		for (int x = 0; x < cornerMap.cols; x++)
		{
			if (rowPtr[x])
			{
				points.push_back(cv::Point(x, y));
			}
		}
	}
}

void HarrisDetector::drawOnImage(cv::Mat & image, const std::vector<cv::Point>& points, cv::Scalar color, int radius, int thickness)
{
	std::vector<cv::Point>::const_iterator iter;
	for (iter = points.cbegin(); iter != points.cend(); iter++)
	{
		cv::circle(image, *iter, radius, color, thickness);
		++iter;
	}

}
