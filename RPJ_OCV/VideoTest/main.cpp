#include <opencv2\core.hpp>
#include <opencv2\video.hpp>
#include <opencv2\videoio.hpp>
#include <opencv2\highgui.hpp>

#include <iostream>

int main(int argc, char** argv)
{
	cv::VideoCapture capture;
	if (argc != 1)
	{
		capture.open(argv[1]);
	}
	else
	{
		capture.open("filename");
	}
	if (!capture.isOpened())
	{
		return 1;
	}

	// Get frame rate.
	double rate = capture.get(cv::CAP_PROP_FPS);
	int delay = 1000 / rate;

	// Get the frame count.
	int rateNum = capture.get(cv::CAP_PROP_FRAME_COUNT);

	// Time long
	double timeLong = rateNum / rate;

	bool stop = false;
	cv::Mat frame;
	cv::namedWindow("Frame");

	double position = 100;
	capture.set(cv::CAP_PROP_POS_FRAMES, position);

	while (!stop)
	{
		if (!capture.read(frame))
		{
			break;
		}
		cv::imshow("Frame", frame);

		int res = cv::waitKey(delay);
		std::cout << res << std::endl;

		if (res < 255)
		{
			stop = true;
		}
	}

	// Close the video file.
	capture.release();
	return 0;
}