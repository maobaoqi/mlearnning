#pragma once
#include <opencv2\core.hpp>

#define PI 3.1415926

class LineFinder
{
private:
	cv::Mat img;
	std::vector<cv::Vec4i> lines;

	double deltaRho;
	double deltaTheta;

	int minVote;
	double minLength;
	double maxGap;

public:
	LineFinder() : deltaRho(1), deltaTheta(PI / 180), minVote(10),
		minLength(0.0), maxGap(0.0) { };
	~LineFinder();

	void setAccResolution(double dRho, double dTheta);
	void setMinVote(int minv);
	void setLineLengthAndGap(double length, double gap);

	void findLines(cv::Mat& binary);
	void drawDetectedLines(cv::Mat &image, cv::Scalar color = cv::Scalar(255, 255, 255));
};

