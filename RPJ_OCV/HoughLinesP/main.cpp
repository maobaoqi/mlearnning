#include <opencv2/imgproc.hpp>
#include <opencv2\highgui.hpp>

#include <opencv2\features2d.hpp>
#include <opencv2\xfeatures2d.hpp>

int main(int argc, char** argv)
{
	cv::Mat originImg(300, 300, CV_8UC3, cv::Scalar(255, 255, 255));

	cv::Point p1(120, 32);
	cv::Point p2(36, 200);
	cv::Point p3(150, 150);

	std::vector<cv::Point> area;
	area.push_back(p1);
	area.push_back(p2);
	area.push_back(p3);

	cv::Mat img(area);

	std::vector<cv::Point> hullpoints;
	cv::convexHull(img, hullpoints);

	cv::Moments mom = cv::moments(img);
	cv::circle(originImg, cv::Point(mom.m10 / mom.m00, mom.m01 / mom.m00), 2, cv::Scalar(0, 0, 255), 2);
	cv::polylines(originImg, hullpoints, true, cv::Scalar(0, 255, 0), 2);

	cv::namedWindow("IMG");
	cv::imshow("IMG", originImg);




	cv::waitKey(0);
	return 0;
}