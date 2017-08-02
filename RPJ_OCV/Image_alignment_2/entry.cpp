#include <iostream>
#include <vector>


#include <opencv2\opencv.hpp>


#include "test_demo.h"
#include "RobustMatcher.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	cv::Mat imgRef = cv::imread("D:\\Desktop\\twoImages\\test (1).tif", cv::IMREAD_GRAYSCALE);

	cv::Mat srcRef = imgRef(cv::Rect(100, 100, imgRef.cols - 100, imgRef.rows - 100));
	cv::resize(srcRef, srcRef, cv::Size(), 0.1, 0.1);

	int i = 1;
	while (i < 3)
	{
		if (i == 1)
		{
			i++;
			continue;
		}

		char name[50];
		sprintf(name, "D:\\Desktop\\twoImages\\test (%d).tif", 31);
		std::string filename(name);

		cv::Mat img = cv::imread(filename, cv::IMREAD_GRAYSCALE);
		cv::Mat src = img(cv::Rect(100, 100, img.cols - 100, img.rows - 100));


		cv::resize(src, src, cv::Size(), 0.1, 0.1);

		std::vector<cv::DMatch> matches;
		std::vector<cv::KeyPoint> keypoints1, keypoints2;
		RobustMatcher rbMatcher;
		cv::Mat fundamentl = rbMatcher.match(src, srcRef, matches, keypoints1, keypoints2);

		cv::Mat result, showImage;
		cv::drawMatches(src, keypoints1, srcRef, keypoints2, matches, result);

		// Transform
		cv::Mat dst;
		cv::warpAffine(img, dst, fundamentl, cv::Size());

		sprintf(name, "D:\\resutls\\res%d.tif", i);
		std::string writename(name);
		cv::imwrite(writename, dst);
		
		std::cout << "Done " << i << std::endl;

		i++;

		cv::imshow(name, result);
		//cv::imshow("src2", dst);
	}
	

	

	waitKey(0);

	return 0;
}