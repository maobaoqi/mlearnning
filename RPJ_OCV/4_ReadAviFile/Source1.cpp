// Writer.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include "opencv2/opencv.hpp"
#include<iostream>
using namespace cv;
using namespace std;
int main()
{
//#if 0
//	Mat img = imread("test1.bmp");  //OpenCV Mat数据结构
//	if (img.empty())
//	{
//		cout << "error";
//		return -1;
//	}
//	//imshow("pp", img);
//	Size frameSize;
//	frameSize.height = 8192;
//	frameSize.width = 8192;
//	VideoWriter writer;
//	writer.open("C:\\1\\test1.avi", CV_FOURCC('I', 'Y', 'U', 'V'), 10, frameSize, true);
//	for (int i = 0; i < 20; ++i)
//	{
//		writer.write(img);
//	}
//	writer.release();
//	waitKey(0);
//
//#else // 0
//
//	double fps = 20;
//	int width = 5120;
//	int height = 3840;
//	char name[500];
//
//	Size frameSize;
//	frameSize.height = 3840;
//	frameSize.width = 5120;
//	CvVideoWriter *writer = cvCreateVideoWriter("C:\\1\\test.avi", CV_FOURCC('I', 'Y', 'U', 'V'), 20, cvSize(width, height), 1);
//	IplImage *image = cvLoadImage("test.bmp");
//
//	int i = 0;
//	cvNamedWindow("show");
//	cvShowImage("show", image);
//	cvWaitKey(0);
//
//	for (int i = 0; i < 100; ++i)
//	{
//		cvWriteFrame(writer, image);
//	}
//
//	cvReleaseVideoWriter(&writer);
//	cvDestroyWindow("show");
//
//#endif

	//cv::VideoCapture cap;
	//cv::Mat frame;
	//
	//cap.open("D:\\Desktop\\data2_stick.avi");
	//if (!cap.isOpened())
	//{
	//	std::cout << "Can not open the file!" << std::endl;
	//}
	//
	//while (cap.read(frame))
	//{
	//	cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
	//
	//	if (frame.empty())
	//	{
	//		continue;
	//	}
	//
	//	std::cout << frame.channels() << std::endl;
	//
	//	cv::namedWindow("AVI");
	//	cv::imshow("AVI", frame);
	//
	//	int k = cv::waitKey(33);
	//	if (k == 27)
	//	{
	//		break;
	//	}
	//}
	
	cv::Mat reference(cv::Size(256, 256), CV_8U);
	reference.setTo(100, cv::noArray());

	cv::imshow("TEST", reference);
	cv::waitKey(0);

	return 0;
}

