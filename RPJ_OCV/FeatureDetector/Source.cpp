#include <opencv2\opencv.hpp>

#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	Mat img1 = imread(argv[1], cv::IMREAD_GRAYSCALE);
	Mat img2 = imread(argv[2], cv::IMREAD_GRAYSCALE);

	if (!(img1.data && img2.data))
	{
		cout << "error reading images" << endl;
		return -1;
	}

	// ORB detect
	cv::Ptr<cv::Feature2D> detector;
	detector = cv::ORB::create(200);
	vector<KeyPoint> keyPoints_1, keyPoints_2;
	Mat descriptors_1, descriptors_2;
	detector->detectAndCompute(img1, noArray(), keyPoints_1, descriptors_1, false);
	detector->detectAndCompute(img2, noArray(), keyPoints_2, descriptors_2, false);

	// Filter keyPoints
	KeyPointsFilter kpFilter;
	//kpFilter.retainBest(keyPoints_1, 20);
	//kpFilter.retainBest(keyPoints_2, 20);

	//drawKeypoints(img1, keyPoints_1, img1, cv::Scalar(0));
	//cv::Mat dst;
	//resize(img1, dst, Size(), 0.25, 0.25);
	//imshow("TEST", dst);
	//waitKey(0);

	struct Diff
	{
		float x;
		float y;
	};
	std::vector<Diff> diff;
	Diff singleDiff;
	for (int i = 0; i < keyPoints_1.size(); i++)
	{
	
		singleDiff.x = abs(keyPoints_1[i].pt.x - keyPoints_2[i].pt.x);
		singleDiff.y = abs(keyPoints_1[i].pt.y - keyPoints_2[i].pt.y);

		diff.push_back(singleDiff);
	}


	// 
	//DMatch matcher;
	//matcher.

	return 0;
}