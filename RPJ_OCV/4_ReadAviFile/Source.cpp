#include <opencv2\video.hpp>
#include <opencv2\highgui.hpp>

#include <iostream>

int main(void)
{
	cv::VideoCapture cap;
	cap.open("D:\\Desktop\\data2_stick.avi");
	if (!cap.isOpened())
	{
		std::cout << "Can not open the file!" << std::endl;
	}

	cv::Mat frame;
	while (cap.read(frame))
	{
		if (frame.empty())
		{
			continue;
		}

		cv::namedWindow("AVI");
		cv::imshow("AVI", frame);

		int k = cv::waitKey(33);
		if (k == 27)
		{
			break;
		}
	}
	

	return 0;
}