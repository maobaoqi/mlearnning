#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>

#include <ctime>
#include <iostream>

int main(int argc, char** argv)
{

	cv::Mat src = cv::imread("D:\\Test Images\\0-2277\\test (1).tif", cv::IMREAD_GRAYSCALE);

	cv::Mat sumMat;

	clock_t t1, t2;
	t1 = clock();
	cv::integral(src, sumMat);

	cv::Mat roi = src(cv::Rect(780, 753, 1542, 1854));
	cv::Scalar scalarValue = cv::sum(roi);
	long sumROIValue = scalarValue(0);

	t2 = clock();


	double minValue, maxValue;
	cv::minMaxIdx(sumMat, &minValue, &maxValue);

	std::cout << "Min Value: " << minValue << "Max Value: " << maxValue << std::endl;
	std::cout << (double)(t2 - t1) / CLOCKS_PER_SEC * 1000 << std::endl;
}