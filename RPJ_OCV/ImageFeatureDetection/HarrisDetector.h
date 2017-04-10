#pragma once
#include <opencv2\core.hpp>

class HarrisDetector
{
private:
	cv::Mat cornerStrength;
	cv::Mat cornerTh;
	cv::Mat localMax;

	int neighbourhood;
	int aperture;
	double k;
	double maxStrength;
	double threshold;
	int nonMaxSize;

	cv::Mat kernel;

public:
	HarrisDetector() : neighbourhood(3), aperture(3), k(0.01),
		maxStrength(0.0), threshold(0.01), nonMaxSize(3) {
	
		//...
	}
	~HarrisDetector();

public:
	void detect(const cv::Mat& image);

	void getCorners(std::vector<cv::Point> &points, double qualityLevel);
	cv::Mat getCornersMap(double qualityLevel);
	void getCornersPoints(std::vector<cv::Point> &points, const cv::Mat &cornerMap);

	void drawOnImage(cv::Mat &image, const std::vector<cv::Point> &points,
		cv::Scalar color = cv::Scalar(255, 255, 255),
		int radius = 3,
		int thickness = 1);
};

