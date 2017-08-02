#include <opencv2\opencv.hpp>
#include <opencv2\imgproc.hpp>
#include <iostream>


#define PI 3.1415926

using namespace std;

static const int MAX_CORNERS = 1000;

void help(char** argv)
{
	cout << "Call: " << argv[0] << " [image1] [image2]" << endl;
	cout << "Demonstrates Pyramid Lucas-Kanade optical flow." << endl;
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		help(argv);
		exit(-1);
	}

	// Initialize, load two images from the file system, and
	// allocate the images and other structure we will need for
	// results.
	//
	cv::Mat img1 = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
	cv::Mat img2 = cv::imread(argv[2], cv::IMREAD_GRAYSCALE);

	cv::Mat imgA = img1(cv::Rect(70, 70, img1.size().width - 70, img1.size().height - 70));
	cv::Mat imgB = img2(cv::Rect(70, 70, img2.size().width - 70, img2.size().height - 70));

	cv::Size img_sz = imgA.size();
	int win_size = 10;
	cv::Mat imgC = imgB;

	// The first thing we need to do is get the features
	// we want to track.
	//
	vector<cv::Point2f> cornersA, cornersB;
	const int MAX_CORNERS = 500;
	cv::goodFeaturesToTrack(
		imgA,
		cornersA,
		MAX_CORNERS,
		0.01,
		5,
		cv::noArray(),
		3,
		false,
		0.04
	);

	//cv::cornerSubPix(
	//	imgA,
	//	cornersA,
	//	cv::Size(win_size, win_size),
	//	cv::Size(-1, -1),
	//	cv::TermCriteria(
	//		cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS,
	//		20,
	//		0.03
	//	)
	//);

	// Call the Lucas Kanade algorithm
	vector<uchar> features_found;
	cv::calcOpticalFlowPyrLK(
		imgA,
		imgB,
		cornersA,
		cornersB,
		features_found,
		cv::noArray(),
		cv::Size(2 * win_size + 1, 2 * win_size + 1),
		5,
		cv::TermCriteria(
			cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS,
			20,
			0.3
		)
	);

	// Now make some image of what we are looking at:
	// Note that if you want to track cornerB further, i.e.
	// pass them as input to the next calcOpticalFlowPyrLK,
	// you would need to "compress" the vector, i.e. exclude points for
	// which features_found[i] == false.
	vector<cv::Point2f> tmpA, tmpB;
	for (int i = 0; i < (int)cornersA.size(); i++)
	{
		if (!features_found[i])
		{
			continue;
		}
		cv::line(imgC, cornersA[i], cornersB[i], cv::Scalar(0), 2, cv::LINE_AA);
		
		tmpA.push_back(cornersA[i]);
		tmpB.push_back(cornersB[i]);
	}

	// Calcuate the move direction according the points.
	vector<cv::Point2f> motionVectorInfo;
	cv::Point2f singleInfo;
	for (int i = 0; i < tmpA.size(); i++)
	{
		float tmp1 = tmpB[i].x - tmpA[i].x;
		float tmp2 = tmpB[i].y - tmpA[i].y;
		
		int length = tmp1*tmp1 + tmp2*tmp2;
		singleInfo.x = sqrt(length);

		float angle;
		if (tmp1 < 1e-2)
		{
			angle = PI / 2;
		}
		else
		{
			angle = tmp2 / tmp1;
		}
		singleInfo.y = angle;

		if (singleInfo.x > 5)
		{
			motionVectorInfo.push_back(singleInfo);
		}
	}

	// Statistics
	float step = 5.0f;
	vector<vector<cv::Point2f>> histogram;
	vector<cv::Point2f> pointsSet;

	while (motionVectorInfo.size() > 0)
	{
		pointsSet.clear();

		for (auto iter = motionVectorInfo.cbegin(); iter != motionVectorInfo.cend();)
		{
			if (iter->x < step)
			{
				pointsSet.push_back(*iter);
				iter = motionVectorInfo.erase(iter);
			}
			else
			{
				iter++;
			}
		}

		histogram.push_back(pointsSet);
		step += 5.0f;
	}

	int maxCountIndex = 0;
	int maxSize = 0;
	for (int i = 0; i < histogram.size(); i++)
	{
		if (maxSize < histogram[i].size())
		{
			maxCountIndex = i;
			maxSize = histogram[i].size();
		}
	}

	// average
	pointsSet = histogram[maxCountIndex];
	float averageLength = 0, averageAngle = 0;
	for (int i = 0; i < pointsSet.size(); i++)
	{
		averageLength += pointsSet[i].x;
		averageAngle += pointsSet[i].y;
	}
	averageLength /= pointsSet.size();
	averageAngle /= pointsSet.size();

	// motion 
	float delta_y = averageLength / (sqrt((1 + averageAngle*averageAngle)));
	float delta_x = delta_y * averageAngle;
	std::cout << "Delta X:" << delta_x << std::endl;
	std::cout << "Delta Y:" << delta_y << std::endl;

	// transfrom
	cv::Mat transformImg(img1.size(), CV_16U, cv::Scalar(0));



	cv::Mat dst;
	cv::resize(imgC, dst, cv::Size(), 0.25, 0.25);

	//cv::imshow("Image A", imgA);
	//cv::imshow("Image B", imgB);
	cv::imshow("LK Optical Flow Example", dst);
	cv::waitKey(0);

	return 0;
}