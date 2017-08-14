#include <iostream>
#include <vector>


#include <opencv2\opencv.hpp>


#include "test_demo.h"
#include "RobustMatcher.h"
#include <ctime>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	clock_t t1, t2;
	t1 = clock();

	int offset = 0;

	cv::Mat imgRef = cv::imread("D:\\Test Images\\Camera2\\test (1).tif", IMREAD_UNCHANGED);
	
	cv::Mat srcRef = imgRef(cv::Rect(offset, offset, imgRef.cols - offset, imgRef.rows - offset));
	cv::resize(srcRef, srcRef, cv::Size(), 0.1, 0.1);

	int i = 2;
	while (i < 3)
	{
		char name[50];
		sprintf(name, "D:\\Test Images\\Camera2\\test (%d).tif", i);
		std::string filename(name);
		
		cv::Mat img = cv::imread(filename, IMREAD_UNCHANGED);
		cv::Mat src = img(cv::Rect(offset, offset, img.cols - offset, img.rows - offset));

		cv::resize(src, src, cv::Size(), 0.1, 0.1);

		std::vector<cv::DMatch> matches;
		std::vector<cv::KeyPoint> keypoints1, keypoints2;
		RobustMatcher rbMatcher;
		cv::Mat fundamentl = rbMatcher.match(src, srcRef, matches, keypoints1, keypoints2);

		cv::Mat result, showImage;
		cv::drawMatches(src, keypoints1, srcRef, keypoints2, matches, result);

		cv::Mat dst;
		cv::warpAffine(img, dst, fundamentl, cv::Size());
		
		sprintf(name, "D:\\resutls\\New folder (3)\\res%d.tif", i);
		std::string writename(name);
		cv::imwrite(writename, dst);
		
		std::cout << "Done " << i << std::endl;

		i++;

		cv::imshow(name, result);
	}
	
	t2 = clock();

	std::cout << double(t2 - t1) / CLOCKS_PER_SEC * 1000 / (i - 1) << std::endl;
	waitKey(0);

	return 0;
}