#include <ippi.h>
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>

#include <iostream>

int main(void)
{
	// IPP to Mat
	Ipp32f* img = new Ipp32f[128 * 128];
	ippiSet_32f_C1R(0.7f, img, 128 * sizeof(Ipp32f), { 128, 128 });

	cv::Mat src(cv::Size(128, 128), CV_32F, img);
	//cv::imshow("IMG", src);


	// Mat to IPP
	cv::Mat img2 = cv::Mat::zeros(cv::Size(512, 512), CV_32F);
	Ipp32f *img2Ptr = (Ipp32f*)img2.data;

	ippiSet_32f_C1R(0.1f, img2Ptr, 512 * sizeof(Ipp32f), { 512, 512 });

	cv::Mat dst(cv::Size(512, 512), CV_32F, img2Ptr);
	//cv::imshow("DST", dst);

	// Convert
	cv::Mat imageSrc(128, 128, CV_16U, cv::Scalar(50000));
	cv::Mat image8u;
	imageSrc.convertTo(image8u, CV_8U, 255.0f / 65535);
	//cv::imshow("C", image8u);

	// IPP Ipp32f format transform into openCV Mat 32F
	Ipp32f *src32f = new Ipp32f[5 * 5];
	ippiSet_32f_C1R(300.2f, src32f, 5 * sizeof(Ipp32f), { 5, 5 });

	cv::Mat srcOpencv32F(cv::Size(5, 5), CV_32F, src32f);
	for (int i = 0; i < 5; i++)
	{
		auto j = srcOpencv32F.ptr<float>(i);
		for (int k = 0; k < 5; k++)
		{
			std::cout << *j << " ";
			j++;
		}
		std::cout << std::endl;
	}


	cv::Mat imgFind(cv::Size(5, 5), CV_8U, cv::Scalar(5));
	imgFind.at<unsigned char>(2, 2) = 0;

	double imin = 0, imax = 0;
	cv::min(imgFind, imin);
	cv::max(imgFind, imax);

	double minValue, maxValue;
	cv::minMaxLoc(imgFind, &minValue, &maxValue);

	std::cout << std::endl;
	std::cout << (double)imin << (double)imax << std::endl;


	Ipp8u *imageIpp8u = new Ipp8u[128 * 128];
	ippiSet_8u_C1R(100, imageIpp8u, 128 * sizeof(Ipp8u), { 128, 128 });

	cv::Mat imageMat8u;
	imageMat8u.create(cv::Size(128, 128), CV_8U);

	imageMat8u.data = imageIpp8u;

	cv::imshow("TEST", imageMat8u);

	cv::waitKey(0);

	return 0;
}