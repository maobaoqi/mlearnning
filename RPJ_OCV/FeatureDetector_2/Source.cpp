#include <opencv2\opencv.hpp>

#include <vector>

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		return -1;
	}

	cv::Mat re0 = imread(argv[1], cv::IMREAD_GRAYSCALE);
	cv::Mat src = imread(argv[2], cv::IMREAD_GRAYSCALE);

	cv::Ptr<cv::Feature2D> detector;
	detector = cv::ORB::create();

	std::vector<cv::KeyPoint> keypoints_1, keypoints_2;
	cv::Mat descriptor_1, descriptor_2;

	detector->detectAndCompute(re0, cv::noArray(), keypoints_1, descriptor_1, false);
	detector->detectAndCompute(src, cv::noArray(), keypoints_2, descriptor_2, false);

	cv::Ptr<cv::DescriptorMatcher> matcher;
	matcher = cv::makePtr<cv::FlannBasedMatcher>(cv::makePtr<cv::flann::LshIndexParams>(12, 20, 2));
	std::vector<cv::DMatch> matches;

	if (descriptor_1.empty() || descriptor_2.empty())
	{
		return -1;
	}

	matcher->match(descriptor_1, descriptor_2, matches, cv::Mat());

	// Find good matcher
	double max_dist = 0.0f;
	double min_dist = 100.0f;
	for (int i = 0; i < matches.size(); i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist)
		{
			min_dist = dist;
		}
		if (dist > max_dist)
		{
			max_dist = dist;
		}
	}

	std::vector<cv::DMatch> good_matches;
	std::vector<cv::KeyPoint> good_keypoints_1, good_keypoints_2;
	for (int i = 0; i < matches.size(); i++)
	{
		if (matches[i].distance < 2 * min_dist)
		{
			good_matches.push_back(matches[i]);
			good_keypoints_1.push_back(keypoints_1[matches[i].queryIdx]);
			good_keypoints_2.push_back(keypoints_2[matches[i].trainIdx]);
		}
	}

	// Transform matrix
	std::vector<cv::Point2f> frame1_features_ok, frame2_features_ok;
	cv::Point2f tmpPoint;

	for (int i = 0; i < good_keypoints_1.size(); i++)
	{
		tmpPoint.x = good_keypoints_1[i].pt.x;
		tmpPoint.y = good_keypoints_1[i].pt.y;
		frame1_features_ok.push_back(tmpPoint);

		tmpPoint.x = good_keypoints_2[i].pt.x;
		tmpPoint.y = good_keypoints_2[i].pt.y;
		frame2_features_ok.push_back(tmpPoint);
	}

	cv::Mat drawMatches;
	cv::drawMatches(re0, good_keypoints_1, src, good_keypoints_2, good_matches, drawMatches);
	cv::resize(drawMatches, drawMatches, cv::Size(), 0.2, 0.2);


	cv::Mat transformMatrix = cv::estimateRigidTransform(frame1_features_ok, frame2_features_ok, true);

	cv::Mat result;
	cv::warpAffine(src, result, transformMatrix, cv::Size());

	cv::Mat showRef, showRes;
	cv::resize(re0, showRef, cv::Size(), 0.1, 0.1);
	cv::resize(result, showRes, cv::Size(), 0.1, 0.1);

	cv::imshow("REF", showRef);
	cv::imshow("RES", showRes);
	cv::waitKey(0);

	return 0;
}
