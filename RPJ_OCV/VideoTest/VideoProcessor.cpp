#include "VideoProcessor.h"



VideoProcessor::VideoProcessor()
{
}


VideoProcessor::~VideoProcessor()
{
}

bool VideoProcessor::setInput(std::string filename)
{
	fnumber = 0;
	capture.release();
	return capture.open(filename);
}

void VideoProcessor::displayInput(std::string wn)
{
	windowNameInput = wn;
	cv::namedWindow(windowNameInput);
}

void VideoProcessor::displayOutput(std::string wn)
{
	windowNameOutput = wn;
	cv::namedWindow(wn);
}

void VideoProcessor::run()
{
	cv::Mat frame;
	cv::Mat output;

	if (!isOpened())
	{
		return;
	}

	stop = false;

	while (!isStopped())
	{
		if (!readNextFrame(frame))
		{
			break;
		}
		if (windowNameInput.length() != 0)
		{
			cv::imshow(windowNameInput, frame);
		}

		if (callIt)
		{
			process(frame, output);
			fnumber++;
		}
		else
		{
			output = frame;
		}

		if (windowNameOutput.length() != 0)
		{
			cv::imshow(windowNameOutput, output);
		}

		if (delay >= 0 && cv::waitKey(delay) < 255)
		{
			stopIt();
		}

		if (frameToStop >= 0 && getFrameNumber() == frameToStop)
		{
			stopIt();
		}
	}
}

void VideoProcessor::stopIt()
{
	stop = true;
}

bool VideoProcessor::isStopped()
{
	return stop;
}

bool VideoProcessor::isOpened()
{
	return capture.isOpened();
}

void VideoProcessor::setDelay(int d)
{
	delay = d
}

bool VideoProcessor::readNextFrame(cv::Mat & frame)
{
	return capture.read(frame);
}

void VideoProcessor::callProcess()
{
	callIt = true;
}

void VideoProcessor::dontCallProcess()
{
	callIt = false;
}

void VideoProcessor::stopAtFrameNo(long frame)
{
	frameToStop = frame;
}

long VideoProcessor::getFrameNumber()
{
	long fnumber = static_cast<long>(capture.get(cv::CAP_PROP_POS_FRAMES));
	return fnumber;
}
