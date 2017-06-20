// 测试左移运算符

#include <iostream>
#include <cmath>

#include <opencv2\core.hpp>
#include <opencv2\imgproc.hpp>

#include <fftw3.h>

#include "opencv_dft_function.h"

int main(int argc, char** argv)
{

	//int fftszieX = int(log2(3840));
	//
	//int value = 1 << 16;
	//std::cout << value << std::endl;
	//
	//
	//cv::Mat img(cv::Size(5120, 3840), CV_32FC1);
	//int orderX = cv::getOptimalDFTSize(img.cols);
	//int orderY = cv::getOptimalDFTSize(img.rows);
	//std::cout << orderX << "  " << orderY << std::endl;
	//
	//cv::Mat dst = img.clone();
	//
	//
	//cv::dft(img, dst);
	//
	//testDFT();

	//cv::Mat img8uc1_testTh(cv::Size(5120, 3840), CV_8UC1);
	//img8uc1_testTh.data[0] = 150;
	//img8uc1_testTh.data[1] = 128;
	//int64 ts = cv::getCPUTickCount();
	//cv::threshold(img8uc1_testTh, img8uc1_testTh, 100, cv::THRESH_BINARY, cv::THRESH_OTSU);
	//int64 te = cv::getCPUTickCount();
	//std::cout << (te - ts) / cv::getTickFrequency() << std::endl;



	// test fftw
	cv::Mat imgSrc = cv::imread("D:\\Desktop\\timg.jpg", cv::IMREAD_GRAYSCALE);
	int numRow = imgSrc.rows;
	int numCol = imgSrc.cols;

	fftw_complex *in, *out;
	in = (fftw_complex*)fftw_malloc(numRow * numCol*sizeof(fftw_complex));
	out = (fftw_complex*)fftw_malloc(numRow * numCol*sizeof(fftw_complex));

	fftw_plan thisPlan;
	thisPlan = fftw_plan_dft_2d(numRow, numCol, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

	// padding value
	for (int i = 0; i < numRow; i++)
	{
		for (int j = 0; j < numCol; j++)
		{
			in[i*numCol + j][0] = imgSrc.at<unsigned int>(i, j);
		}
	}



	fftw_execute(thisPlan);



	fftw_free(in);
	fftw_free(out);
	fftw_destroy_plan(thisPlan);

	return 0;
}