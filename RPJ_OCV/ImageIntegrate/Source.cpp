#include <opencv2\core.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>


#include <iostream>

int main(int argc, char** argv)
{
	cv::Mat img = cv::imread("D:\\Test Images\\0-2277\\test (1).tif", cv::IMREAD_GRAYSCALE);
	cv::Mat imgInter;
	
	int64 t1 = cv::getTickCount();
	cv::integral(img, imgInter);
	int64 t2 = cv::getTickCount();

	std::cout << (double)(t2 - t1) / cv::getTickFrequency() << std::endl;

	// choose roi
	int x = 200, y = 200;
	int w = 600, h = 500;
	cv::Mat imgROI = img(cv::Rect(x, y, w, h));
	int64 sumROI = cv::sum(imgROI)[0];

	// read query image
	cv::Mat queryImg = cv::imread("D:\\Test Images\\0-2277\\test (2).tif", cv::IMREAD_GRAYSCALE);
	cv::Mat queryImgInter;
	cv::integral(queryImg, queryImgInter);

	// result mat
	cv::Mat result(cv::Size(queryImg.cols - w, queryImg.rows - h), CV_32S);
	std::vector<int> sumVec;

	// scan queryImgInter
	for (int i = h / 2; i < (queryImgInter.rows - h / 2); i++)
	{
		//int* data = result.ptr<int>(i - h/2);
		int* idata1 = queryImgInter.ptr<int>(i - (h / 2));
		int* idata2 = queryImgInter.ptr<int>(i + (h / 2));

		for (int j = w / 2; j < (queryImgInter.cols - w / 2); j++)
		{
			int sum = (idata2)[j + w / 2] - (idata2)[j - w / 2] - (idata1)[j + w / 2] + (idata1)[j - w / 2];
			//data[j] = abs(sum - sumROI);
			sumVec.push_back(sum);
		}
	}


	return 0;
}