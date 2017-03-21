#include <iostream>

#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>

int main(int argc, char** argv)
{
	cv::Mat zeroImg(100, 100, CV_8UC1);
	cv::namedWindow("Test");
	cv::imshow("Test", zeroImg);
	
	cv::waitKey(0);
	return 0;
}