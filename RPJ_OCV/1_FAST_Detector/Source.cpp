#include <opencv2\core.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>

#include <opencv2\opencv.hpp>

#include <iostream>
#include <vector>
#include <ctime>

int main(void)
{
	cv::Mat img = cv::imread("D:\\Test Images\\0-2277\\test (1).tif", cv::IMREAD_GRAYSCALE);
	if (img.channels() == 3)
	{
		cv::cvtColor(img, img, cv::COLOR_RGB2GRAY);
	}
	cv::GaussianBlur(img, img, cv::Size(3, 3), 2.5, 2.5);

	cv::Ptr<cv::Feature2D> featuresDetector;
	featuresDetector = cv::ORB::create();
	std::vector<cv::KeyPoint> keypoints;
	
	clock_t t1, t2;
	t1 = clock();
	featuresDetector->detect(img, keypoints, cv::noArray());
	t2 = clock();
	std::cout << "Time: " << (double)(t2 - t1) / CLOCKS_PER_SEC * 1000 << std::endl;
	std::cout << "KeyPoint number: " << keypoints.size() << std::endl;

	cv::Ptr<cv::DescriptorExtractor> descriptor;
	cv::Mat descriptorMatrix;
	featuresDetector->compute()
	//featuresDetector->

	cv::Mat drawImg;
	cv::drawKeypoints(img, keypoints, drawImg, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT);
	cv::resize(drawImg, drawImg, cv::Size(), 0.2, 0.2);
	cv::imshow("KeyPoints", drawImg);
	cv::waitKey(0);
	
	return 0;
}