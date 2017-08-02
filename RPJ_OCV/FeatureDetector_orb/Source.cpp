#include <opencv2\opencv.hpp>

#include <iostream>
#include <vector>

int main(int argc, char** argv)
{
	// Load two images
	cv::Mat img1 = imread(argv[1], cv::IMREAD_GRAYSCALE);
	cv::Mat img2 = imread(argv[2], cv::IMREAD_GRAYSCALE);

	cv::Mat src1, src2;
	src1 = img1(cv::Rect(100, 100, img1.cols - 100, img1.rows - 100));
	src2 = img2(cv::Rect(100, 100, img2.cols - 100, img2.rows - 100));

	if (src1.empty() || src1.empty())
	{
		std::cout << "Can not load the images." << std::endl;
	}

	// Feature detect and match
	cv::Ptr<cv::Feature2D> detector;
	detector = cv::ORB::create(300, 1.6, 3, 35, 0, 2, 0, 500, 30);

	std::vector<cv::KeyPoint> keyPoints1, keyPoints2;
	cv::Mat descriptorArray1, descriptorArray2;
	detector->detectAndCompute(src1, cv::noArray(), keyPoints1, descriptorArray1, false);
	detector->detectAndCompute(src2, cv::noArray(), keyPoints2, descriptorArray2, false);

	cv::Ptr<cv::DescriptorMatcher> descriptorMatcher;
	descriptorMatcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

	std::vector<cv::DMatch> matches;
	descriptorMatcher->match(descriptorArray1, descriptorArray2, matches, cv::noArray());

	// Find good matches
	cv::Mat index;
	int nbMatch = int(matches.size());
	cv::Mat tab(nbMatch, 1, CV_32F);
	for (int i = 0; i < nbMatch; i++)
	{
		tab.at<float>(i, 0) = matches[i].distance;
	}
	cv::sortIdx(tab, index, cv::SORT_EVERY_COLUMN + cv::SORT_ASCENDING);

	std::vector<cv::DMatch> goodMatches;
	for (int i = 0; i < 10; i++)
	{
		goodMatches.push_back(matches[index.at<int>(i, 0)]);
	}

	// Show
	cv::Mat result;
	cv::drawMatches(src1, keyPoints1, src2, keyPoints2, goodMatches, result, 
		cv::Scalar(0, 0, 255), cv::Scalar(255, 0, 0));

	cv::Mat show;
	cv::resize(result, show, cv::Size(), 0.2, 0.2);
	cv::imshow("SHOW", show);
	cv::waitKey(0);

	// Get transform matrix
	std::vector<cv::Point2f> points1, points2;
	cv::Point2f tempPoint;
	for (auto match : goodMatches)
	{
		points1.push_back(keyPoints1[match.queryIdx].pt);
		points2.push_back(keyPoints2[match.trainIdx].pt);
	}

	cv::Mat tm = cv::estimateRigidTransform(points1, points2, false);

	for (int i = 0; i < tm.rows * tm.cols; i++)
	{
		std::cout << (float)tm.data[i] << " ";
		if ((i + 1) % tm.cols == 0)
		{
			std::cout << std::endl;
		}
	}

	cv::warpAffine(img1, show, tm, cv::Size());

	cv::imwrite("D:\\Desktop\\twoImages\\corrected.tif", show);

	// Apply transform
	cv::resize(show, result, cv::Size(), 0.2, 0.2);
	cv::imshow("RES", result);
	cv::waitKey(0);

	return 0;
}