#include <opencv2\core.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>

int main(int argc, char** argv)
{
	cv::Mat image;
	image = cv::imread("E:\\OpenCV\\Source3v\\opencv-3.2.0\\samples\\data\\blox.jpg");
	if (image.empty())
	{
		return -1;
	}

	if (image.channels() == 3)
	{
		cv::cvtColor(image, image, cv::COLOR_BGRA2GRAY);
	}

	cv::Mat cornerStrength;
	cv::cornerHarris(image, cornerStrength, 3, 3, 0.01);

	cv::Mat harrisCorners;
	double threshold = 0.0001;
	cv::threshold(cornerStrength, harrisCorners, threshold, 255, cv::THRESH_BINARY);

	cv::namedWindow("Harris Strength");
	cv::imshow("Harris Strength", harrisCorners);
	cv::waitKey(0);

	std::vector<cv::KeyPoint> keypoints;
	cv::Ptr<cv::FastFeatureDetector> fd1 = new cv::FastFeatureDetector();

	return 0;
}