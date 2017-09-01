#include <iostream>

#include <opencv2\highgui.hpp>

#include "OpticalFlowDetectMotion.h"

int main(void)
{
	cv::Mat referenceImage;
	referenceImage = cv::imread("D:\Desktop\data5_BF\\test (3).tif", cv::IMREAD_GRAYSCALE);

	OpticalFlowDetectMotion detectMotion;
	detectMotion.SetReferenceImage(referenceImage);

	int i = 1;
	while (i < 7)
	{
		cv::Mat queryImage;
		char inputName[50];
		sprintf_s(inputName, "D:\Desktop\data5_BF\\test (%d).tif", i);
		queryImage = cv::imread(inputName, cv::IMREAD_GRAYSCALE);

		cv::Mat matrixT;
		detectMotion.CalMotion(queryImage, matrixT);

		cv::Mat dst;
		cv::warpAffine(queryImage, dst, matrixT, cv::Size());

		char name[50];
		sprintf_s(name, "D:\\opticalFlow_test%d.tif", i);
		cv::imwrite(std::string(name), dst);

		i++;
	}


	return 0;
}