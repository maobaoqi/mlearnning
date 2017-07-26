#include <opencv2\core.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
	char* filename = nullptr;
	if (argc == 2)
	{
		filename = argv[1];
	}
	else
	{
		filename = nullptr;
	}

	cv::Mat imageSrc;
	if (filename != nullptr)
	{
		imageSrc = cv::imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
	}
	else
	{
		imageSrc.create(cv::Size(451, 451), CV_8U);
		imageSrc.setTo(100, cv::Mat());
	}

	cv::imshow("Image Src", imageSrc);
	
	// add window
	cv::Mat hann;
	cv::createHanningWindow(hann, imageSrc.size(), CV_32F);
	imageSrc.convertTo(imageSrc, CV_32F);
	cv::multiply(imageSrc, hann, imageSrc);

	cv::Mat showImg;
	imageSrc.convertTo(showImg, CV_8U);

	cv::imshow("src image", imageSrc);
	
	// fft
	cv::Mat padded;
	int m = cv::getOptimalDFTSize(imageSrc.rows);
	int n = cv::getOptimalDFTSize(imageSrc.cols);
	//int m = imageSrc.rows;
	//int n = imageSrc.cols;
	std::cout << "m optimal size: " << m << std::endl;
	std::cout << "n optimal size: " << n << std::endl;

	std::cout << "imageSrc rows: " << imageSrc.rows << std::endl;
	std::cout << "imageSrc cols:" << imageSrc.cols << std::endl;

	m = 2048;

	cv::copyMakeBorder
	(
		imageSrc, 
		padded, 
		0, 
		m - imageSrc.rows, 
		0, 
		n - imageSrc.cols, 
		cv::BORDER_CONSTANT, 
		cv::Scalar(0)
	);

	cv::Mat complexSrc;
	cv::Mat planes[] = { cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F) };
	cv::merge(planes, 2, complexSrc);

	int64 time0 = cv::getTickCount();

	cv::dft(complexSrc, complexSrc);

	int64 time1 = cv::getTickCount();

	std::cout << "Time consum: " << (time1 - time0) / cv::getTickFrequency() << std::endl;

	// magnitude
	cv::split(complexSrc, planes);
	cv::magnitude(planes[0], planes[1], planes[0]);
	cv::Mat magnitudeSrc = planes[0];

	magnitudeSrc += cv::Scalar::all(1);
	cv::log(magnitudeSrc, magnitudeSrc);

	// cutoff spectrum
	magnitudeSrc = magnitudeSrc(cv::Rect(0, 0, magnitudeSrc.cols&-2, magnitudeSrc.rows&-2));

	// rearrage
	int cx = magnitudeSrc.cols / 2;
	int cy = magnitudeSrc.rows / 2;

	cv::Mat s0(magnitudeSrc, cv::Rect(0, 0, cx, cy));
	cv::Mat s1(magnitudeSrc, cv::Rect(cx, 0, cx, cy));
	cv::Mat s2(magnitudeSrc, cv::Rect(0, cy, cx, cy));
	cv::Mat s3(magnitudeSrc, cv::Rect(cx, cy, cx, cy));

	cv::Mat tmp;
	s0.copyTo(tmp);
	s3.copyTo(s0);
	tmp.copyTo(s3);

	s2.copyTo(tmp);
	s1.copyTo(s2);
	tmp.copyTo(s1);

	// normalize
	cv::normalize(magnitudeSrc, magnitudeSrc, 0, 1, CV_MINMAX);

	cv::imshow("Spectrum Image", magnitudeSrc);

	cv::waitKey(0);
	return 0;
}