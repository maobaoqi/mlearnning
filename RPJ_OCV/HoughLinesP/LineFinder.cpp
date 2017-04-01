#include "LineFinder.h"

#include <opencv2\imgproc.hpp>


LineFinder::~LineFinder()
{
}

void LineFinder::setAccResolution(double dRho, double dTheta)
{
	deltaRho = dRho;
	deltaTheta = dTheta;
}

void LineFinder::setMinVote(int minv)
{
	minVote = minv;
}

void LineFinder::setLineLengthAndGap(double length, double gap)
{
	minLength = length;
	maxGap = gap;
}

void LineFinder::findLines(cv::Mat & binary)
{
	lines.clear();
	cv::HoughLinesP(binary, lines, deltaRho, deltaTheta, minVote, minLength, maxGap);
}

void LineFinder::drawDetectedLines(cv::Mat & image, cv::Scalar color)
{
	std::vector<cv::Vec4i>::const_iterator it = lines.cbegin();
	while (it != lines.cend())
	{
		cv::Point pt1((*it)[0], (*it)[1]);
		cv::Point pt2((*it)[2], (*it)[3]);

		cv::line(image, pt1, pt2, color);

		it++;
	}
}
