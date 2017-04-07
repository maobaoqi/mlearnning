#pragma once
#include <opencv2\videoio.hpp>
#include <opencv2\video.hpp>
#include <opencv2\highgui.hpp>

class VideoProcessor
{
public:
	VideoProcessor();
	~VideoProcessor();

private:
	cv::VideoCapture capture;
	void(*process)(cv::Mat&, cv::Mat&);
	bool callIt;
	std::string windowNameInput;
	std::string windowNameOutput;

	int delay;
	long fnumber;
	long frameToStop;
	bool stop;

public:
	void setFrameProcessor(void(*frameProcessingCallback) (cv::Mat&, cv::Mat&))
	{
		process = frameProcessingCallback;
	}

	bool setInput(std::string filename);
	void displayInput(std::string wn);
	void displayOutput(std::string wn);
	void run();
	void stopIt();
	bool isStopped();
	bool isOpened();
	void setDelay(int d);
	bool readNextFrame(cv::Mat& frame);
	void callProcess();
	void dontCallProcess();
	void stopAtFrameNo(long frame);
	long getFrameNumber();
};

