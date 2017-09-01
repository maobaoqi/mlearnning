#include <opencv2\features2d.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\calib3d.hpp>

#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	Mat src = imread("D:\\Test Images\\Camera2\\test (1).tif", cv::IMREAD_UNCHANGED);
	Mat ref = imread("D:\\Test Images\\Camera2\\test (2).tif", cv::IMREAD_UNCHANGED);

	//resize(src, src, Size(), 0.5, 0.5);
	//resize(ref, ref, Size(), 0.5, 0.5);
	int r = src.rows;
	int c = src.cols;
	r = r / 200;
	c = c / 200;
	if (r % 2 == 0)
	{
		r += 1;
	}
	if (c % 2 == 0)
	{
		c += 1;
	}

	cv::GaussianBlur(src, src, cv::Size(c, r), 2.5, 2.5);
	cv::GaussianBlur(ref, ref, cv::Size(c, r), 2.5, 2.5);

	Ptr<Feature2D> detector;
	detector = ORB::create();

	vector<KeyPoint> keypoints_src, keypoints_ref;
	Mat descriptors_src, descriptors_ref;
	//detector->detectAndCompute(src, noArray(), keypoints_src, descriptors_src, false);
	//detector->detectAndCompute(ref, noArray(), keypoints_ref, descriptors_ref, false);

	detector->detect(src, keypoints_src, noArray());
	detector->detect(ref, keypoints_ref, noArray());
	
	Ptr<Feature2D> computor;
	computor = BRISK::create();
	computor->compute(src, keypoints_src, descriptors_src);
	computor->compute(ref, keypoints_ref, descriptors_ref);
	vector<DMatch> matches;
	Ptr<DescriptorMatcher> matcher;
	matcher = BFMatcher::create(NORM_HAMMING, true);
	matcher->add(descriptors_ref);
	matcher->match(descriptors_src, matches, noArray());
	//matcher->radiusMatch(des)

	vector<Point2f> points_src, points_ref;
	auto iterMatch = matches.cbegin();
	while (iterMatch != matches.cend())
	{
		points_src.push_back(keypoints_src[iterMatch->queryIdx].pt);
		points_ref.push_back(keypoints_ref[iterMatch->trainIdx].pt);
		iterMatch++;
	}
	vector<uchar> mask(points_src.size(), 0);
	findFundamentalMat(points_src, points_ref, mask);
	auto itIn = mask.cbegin();
	auto itMa = matches.cbegin();
	vector<DMatch> goodMatches;
	while (itIn != mask.cend())
	{
		if (*itIn)
		{
			goodMatches.push_back(*itMa);
		}
		itIn++;
		itMa++;
	}

	Mat index;
	int nbMatch = int(goodMatches.size());
	Mat tab(nbMatch, 1, CV_32F);
	for (int i = 0; i < nbMatch; i++)
	{
		tab.at<float>(i, 0) = goodMatches[i].distance;
	}
	sortIdx(tab, index, SORT_ASCENDING + SORT_EVERY_COLUMN);
	vector<DMatch> goodMatches_2;
	for (int i = 0; i < goodMatches.size(); i++)
	{
		goodMatches_2.push_back(goodMatches[index.at<int>(i, 0)]);
	}
	
	//drawKeypoints(src, keypoints_src, src, Scalar(0, 0, 255));
	//drawKeypoints(ref, keypoints_ref, ref, Scalar(0, 0, 255));
	Mat matchResult;
	drawMatches(src, keypoints_src, ref, keypoints_ref, goodMatches_2, matchResult);
	resize(matchResult, matchResult, cv::Size(), 0.5, 0.5);
	imshow("RES", matchResult);

	//resize(src, src, Size(), 0.5, 0.5);
	//resize(ref, ref, Size(), 0.5, 0.5);
	//imshow("src", src);
	//imshow("ref", ref);

	// BF Match
	//vector<DMatch> matches;
	//Ptr<DescriptorMatcher> matcher;
	//matcher = cv::BFMatcher::create(NORM_HAMMING, true);
	//matcher->match(descriptors_ref, descriptors_src, matches, noArray());
	//matcher->radiusMatch(descriptors_ref, descriptors_src, matches, 20, noArray(), );
	
	// Flann Match
	//Ptr<DescriptorMatcher> matcher;
	//matcher = cv::FlannBasedMatcher::create();
	//vector<DMatch> matches_flann;
	//matcher->match(descriptors_ref, descriptors_src, matches_flann, noArray());


	//cv::Mat outShow;
	//drawMatches(src, keypoints_src, ref, keypoints_ref, matches, outShow);
	//resize(outShow, outShow, cv::Size(), 0.4, 0.4);
	//imshow("TEST", outShow);
	cv::waitKey(0);

	return 0;
}