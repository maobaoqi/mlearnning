#include "RobustMatcher.h"

cv::Mat RobustMatcher::match(cv::Mat image1, cv::Mat image2,
	std::vector<cv::DMatch> &matches,
	std::vector<cv::KeyPoint> &keypoints1,
	std::vector<cv::KeyPoint> &keypoints2)
{
	cv::Mat descriptors1, descriptors2;


	detector->detect(image1, keypoints1, cv::noArray());
	detector->detect(image2, keypoints2, cv::noArray());

	computor->compute(image1, keypoints1, descriptors1);
	computor->compute(image2, keypoints2, descriptors2);

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
	struct pointAndIndex
	{
		cv::Point2f point;
		int index;
	};

	std::vector<pointAndIndex> pointsInfo1, pointsInfo2;
	pointAndIndex pointInfo;

	std::vector<cv::Point2f> points1, points2;
	std::vector<cv::DMatch>::const_iterator it = matches.cbegin();
	while (it != matches.cend())
	{
		points1.push_back(keypoints1[it->queryIdx].pt);
		points2.push_back(keypoints2[it->trainIdx].pt);

		pointInfo.point = keypoints1[it->queryIdx].pt;
		pointInfo.index = it->queryIdx;
		pointsInfo1.push_back(pointInfo);

		pointInfo.point = keypoints2[it->trainIdx].pt;
		pointInfo.index = it->trainIdx;
		pointsInfo2.push_back(pointInfo);

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
	for (int i = 0; i < outMatches.size(); i++)
	{
		goodMatches.push_back(outMatches[index.at<int>(i, 0)]);
	}

	std::swap(goodMatches, outMatches);

	if (true)
	{
		pointsInfo1.clear();
		pointsInfo2.clear();
	
		std::vector<cv::DMatch>::const_iterator it = outMatches.cbegin();
		
		while (it != outMatches.cend())
		{
			pointInfo.point = keypoints1[it->queryIdx].pt;
			pointInfo.index = it->queryIdx;
			pointsInfo1.push_back(pointInfo);

			pointInfo.point = keypoints2[it->trainIdx].pt;
			pointInfo.index = it->trainIdx;
			pointsInfo2.push_back(pointInfo);
	
			it++;
		}

		struct MotionVector
		{
			float motionLength;
			float motionDirect;
			float xDifferValue;
			float yDifferValue;

			pointAndIndex points1;
			pointAndIndex points2;
		};

		std::vector<MotionVector> motionVector;
		MotionVector singleMotionVector;

		for (int i = 0; i < pointsInfo1.size(); i++)
		{
			singleMotionVector.xDifferValue = pointsInfo1[i].point.x - pointsInfo2[i].point.x;
			singleMotionVector.yDifferValue = pointsInfo1[i].point.y - pointsInfo2[i].point.y;

			if (abs(singleMotionVector.xDifferValue) < 1e-5)
			{
				if (abs(singleMotionVector.yDifferValue) < 1e-5)
				{
					singleMotionVector.motionDirect = 0.0f;
				}
				else if (singleMotionVector.yDifferValue < -(1e-5))
				{
					singleMotionVector.motionDirect = -3.1415926 / 2;
				}
				else if (singleMotionVector.yDifferValue > 1e-5)
				{
					singleMotionVector.motionDirect = 3.1415926 / 2;
				}
			}
			else
			{
				singleMotionVector.motionDirect = std::atanf(singleMotionVector.yDifferValue / singleMotionVector.xDifferValue);
			}

			singleMotionVector.motionLength = sqrt(singleMotionVector.xDifferValue*singleMotionVector.xDifferValue 
				+ singleMotionVector.yDifferValue * singleMotionVector.yDifferValue);

			singleMotionVector.points1 = pointsInfo1[i];
			singleMotionVector.points2 = pointsInfo2[i];
			
			motionVector.push_back(singleMotionVector);
		}
		
		// Statistics===================================================//
		float step = -3.1415926/2;// -pi/2
		std::vector<std::vector<MotionVector>> motionVectorHistogram;
		std::vector<MotionVector> motionVectorSet;
		
		while (motionVector.size() > 0)
		{
			motionVectorSet.clear();
		
			for (auto iter = motionVector.begin(); iter != motionVector.end();)
			{
				if (iter->motionDirect < step)
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
			step += 0.4f;// pi/8
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
		
		motionVectorSet = motionVectorHistogram[maxCountIndex];
		
		

		//
		step = 0.0f;
		motionVectorHistogram.clear();
		while (motionVectorSet.size() > 0)
		{
			motionVector.clear();

			for (auto iter = motionVectorSet.begin(); iter != motionVectorSet.end();)
			{
				if (iter->motionLength < (step + 1e-1))
				{
					motionVector.push_back(*iter);
					iter = motionVectorSet.erase(iter);
				}
				else
				{
					iter++;
				}
			}

			motionVectorHistogram.push_back(motionVector);
			step += 3.0f;
		}

		maxCountIndex = 0;
		maxSize = 0;
		for (int i = 0; i < motionVectorHistogram.size(); i++)
		{
			if (maxSize < motionVectorHistogram[i].size())
			{
				maxCountIndex = i;
				maxSize = motionVectorHistogram[i].size();
			}
		}

		motionVectorSet.clear();
		motionVectorSet = motionVectorHistogram[maxCountIndex];

		// Update mathces by rotate angle to find the max
		goodMatches.clear();
		for (int i = 0; i < motionVectorSet.size(); i++)
		{
			for (int j = 0; j < outMatches.size(); j++)
			{
				if (outMatches[j].queryIdx == motionVectorSet[i].points1.index)
				{
					goodMatches.push_back(outMatches[j]);
				}
			}
		}
		std::swap(goodMatches, outMatches);



		float avgLength = 0.0f, avgAngle = 0.0, avgDiffX = 0.0, avgDiffY = 0.0;
		for (int i = 0; i < motionVectorSet.size(); i++)
		{
			avgDiffX += motionVectorSet[i].xDifferValue;
			avgDiffY += motionVectorSet[i].yDifferValue;
			
		}
		avgDiffX /= motionVectorSet.size();
		avgDiffY /= motionVectorSet.size();
		
		// motion 
		float delta_y = avgDiffY;
		float delta_x = avgDiffX;
	
		points1.clear();
		points2.clear();
		for (int i = 0; i < motionVectorSet.size(); i++)
		{
			points1.push_back(motionVectorSet[i].points1.point);
			points2.push_back(motionVectorSet[i].points2.point);
		}
	    
		cv::Mat H = cv::Mat(2, 3, CV_64F);
		H.at<double>(0, 0) = 1.0f;
		H.at<double>(0, 1) = 0.0f;
		H.at<double>(0, 2) = -delta_x*2;
		
		H.at<double>(1, 0) = 0.0f;
		H.at<double>(1, 1) = 1.0f;
		H.at<double>(1, 2) = -delta_y*2;

		std::cout << "Delta X: " << -delta_x*2 << "  " << "Delta Y:" << -delta_y*2 << std::endl;

		fundamental = H;
	}


	return fundamental;
}