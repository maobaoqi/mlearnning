#include "RobustMatcher.h"

cv::Mat RobustMatcher::match(cv::Mat image1, cv::Mat image2,
	std::vector<cv::DMatch> &matches,
	std::vector<cv::KeyPoint> &keypoints1,
	std::vector<cv::KeyPoint> &keypoints2)
{
	cv::Mat descriptors1, descriptors2;
	detector->detectAndCompute(image1, cv::noArray(), keypoints1, descriptors1, false);
	detector->detectAndCompute(image2, cv::noArray(), keypoints2, descriptors2, false);

	cv::Ptr<cv::DescriptorMatcher> matcher;
	matcher = cv::BFMatcher::create(normType, true);

	std::vector<cv::DMatch> outputMatches;
	matcher->match(descriptors1, descriptors2, outputMatches, cv::noArray());

	cv::Mat fundamental = ransacTest(outputMatches, keypoints1, keypoints2, matches);

	return fundamental;
}


cv::Mat RobustMatcher::ransacTest
(
	const std::vector<cv::DMatch> &matches,
	const std::vector<cv::KeyPoint> &keypoints1,
	const std::vector<cv::KeyPoint> &keypoints2,
	std::vector<cv::DMatch> &outMatches
)
{
	std::vector<cv::Point2f> points1, points2;
	std::vector<cv::DMatch>::const_iterator it = matches.cbegin();
	while (it != matches.cend())
	{
		points1.push_back(keypoints1[it->queryIdx].pt);
		points2.push_back(keypoints2[it->trainIdx].pt);
		it++;
	}

	std::vector<uchar> inliers(points1.size(), 0);
	cv::Mat fundamental = cv::findFundamentalMat
	(
		points1,
		points2,
		inliers,
		CV_FM_RANSAC,
		distance,
		confidence
	);

	std::vector<uchar>::const_iterator itIn = inliers.cbegin();
	std::vector<cv::DMatch>::const_iterator itM = matches.begin();

	while (itIn != inliers.cend())
	{
		if (*itIn)
		{
			outMatches.push_back(*itM);
		}

		itIn++;
		itM++;
	}

	cv::Mat index;
	int nbMatch = int(outMatches.size());
	cv::Mat tab(nbMatch, 1, CV_32F);
	for (int i = 0; i < nbMatch; i++)
	{
		tab.at<float>(i, 0) = outMatches[i].distance;
	}
	cv::sortIdx(tab, index, cv::SORT_EVERY_COLUMN + cv::SORT_ASCENDING);

	std::vector<cv::DMatch> goodMatches;
	for (int i = 0; i < outMatches.size() / 4; i++)
	{
		goodMatches.push_back(outMatches[index.at<int>(i, 0)]);
	}

	std::swap(goodMatches, outMatches);

	if (refineF)
	{
		points1.clear();
		points2.clear();
	
		std::vector<cv::DMatch>::const_iterator it = outMatches.cbegin();
		
		//int i = 0;
		while (it != outMatches.cend())
		{
			points1.push_back(keypoints1[it->queryIdx].pt);
			points2.push_back(keypoints2[it->trainIdx].pt);
	
			it++;
			//i++;
		}

		// Calcuate the move direction according the points.============
		//std::vector<cv::Point2f> motionVectorInfo;
		//cv::Point2f singleInfo;

		struct MotionVector
		{
			float motionLength;
			float motionAngle;
			float xDifferValue;
			float yDifferValue;
		};

		std::vector<MotionVector> motionVector;
		MotionVector singleMotionVector;

		for (int i = 0; i < points1.size(); i++)
		{
			singleMotionVector.xDifferValue = points1[i].x - points2[i].x;
			singleMotionVector.yDifferValue = points1[i].y - points2[i].y;
		
			singleMotionVector.motionLength = (singleMotionVector.xDifferValue)*(singleMotionVector.xDifferValue) +
				(singleMotionVector.yDifferValue)*(singleMotionVector.yDifferValue);
		
			float angle;
			if (singleMotionVector.xDifferValue < 1e-2)
			{
				singleMotionVector.motionAngle = 3.14 / 2;
			}
			else
			{
				singleMotionVector.motionAngle = singleMotionVector.yDifferValue / singleMotionVector.xDifferValue;
			}
		
			if (singleMotionVector.motionLength > 1.0f)
			{
				motionVector.push_back(singleMotionVector);
			}
		}
		
		// Statistics
		float step = 0.5f;
		std::vector<std::vector<MotionVector>> motionVectorHistogram;
		std::vector<MotionVector> motionVectorSet;
		
		while (motionVector.size() > 0)
		{
			motionVectorSet.clear();
		
			for (auto iter = motionVector.cbegin(); iter != motionVector.cend();)
			{
				if (iter->motionLength < step)
				{
					motionVectorSet.push_back(*iter);
					iter = motionVector.erase(iter);
				}
				else
				{
					iter++;
				}
			}
		
			motionVectorHistogram.push_back(motionVectorSet);
			step += 5.0f;
		}
		
		int maxCountIndex = 0;
		int maxSize = 0;
		for (int i = 0; i < motionVectorHistogram.size(); i++)
		{
			if (maxSize < motionVectorHistogram[i].size())
			{
				maxCountIndex = i;
				maxSize = motionVectorHistogram[i].size();
			}
		}
		
		// average
		motionVectorSet = motionVectorHistogram[maxCountIndex];
		float avgLength = 0.0f, avgAngle = 0.0, avgDiffX = 0.0, avgDiffY = 0.0;
		for (int i = 0; i < motionVectorSet.size(); i++)
		{
			avgLength += motionVectorSet[i].motionLength;
			avgAngle += motionVectorSet[i].motionAngle;
			avgDiffX += motionVectorSet[i].xDifferValue;
			avgDiffY += motionVectorSet[i].yDifferValue;
		}
		avgLength /= motionVectorSet.size();
		avgAngle /= motionVectorSet.size();
		avgDiffX /= motionVectorSet.size();
		avgDiffY /= motionVectorSet.size();
		
		// motion 
		float delta_y = avgDiffY;
		float delta_x = avgDiffX;
	
	    //cv::Mat R = cv::getAffineTransform(points1, points2);
		cv::Mat R = cv::estimateRigidTransform(points1, points2, false);
		//fundamental = cv::findHomography(points1, points2);
		//cv::Mat R = cv::estimateAffine2D(points1, points2, cv::noArray());

		//cv::Mat rotateM = cv::get


		// extend rigid transformation to use perspectiveTransform:
		cv::Mat H = cv::Mat(2, 3, CV_64F);
		H.at<double>(0, 0) = 1.0f;
		H.at<double>(0, 1) = R.at<double>(0, 1);
		H.at<double>(0, 2) = -delta_x * 10;

		H.at<double>(1, 0) = R.at<double>(1, 0);
		H.at<double>(1, 1) = 1.0f;
		H.at<double>(1, 2) = -delta_y * 10;

		//H.at<double>(2, 0) = 0.0f;
		//H.at<double>(2, 1) = 0.0f;
		//H.at<double>(2, 2) = 1.0f;

		std::cout << "Delta X: " << delta_x*10 << "  " << "Delta Y:" << delta_y*10 << std::endl;

		fundamental = H;
	}



	return fundamental;
}