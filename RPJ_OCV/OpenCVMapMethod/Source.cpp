#include <opencv2\reg\mapshift.hpp>
#include <opencv2\reg\mappergradshift.hpp>
#include <opencv2\reg\mapperpyramid.hpp>

#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>

#include <iostream>


static const char* DIFF_IM = "image difference";
static const char* DIFF_REGPIX_IM = "image difference: pixel registered";


static void showDifference(const cv::Mat& image1, const cv::Mat& image2, const char* title)
{
	cv::Mat imgDiff;
	image1.copyTo(imgDiff);
	imgDiff -= image2;
	imgDiff /= 2.f;
	imgDiff += 128.f;

	cv::Mat imgSh;
	imgDiff.convertTo(imgSh, CV_8U);
	cv::imshow(title, imgSh);
}


cv::Scalar SSIM(const cv::Mat &i1, const cv::Mat &i2)
{
	const double C1 = 6.5025, C2 = 58.5225;
	int d = CV_32F;

	cv::Mat I1, I2;
	i1.convertTo(I1, d);
	i2.convertTo(I2, d);

	cv::Mat I2_2 = I2.mul(I2);        // I2^2
	cv::Mat I1_2 = I1.mul(I1);        // I1^2
	cv::Mat I1_I2 = I1.mul(I2);       // I1 * I2

	cv::Mat mu1, mu2;   // PRELIMINARY COMPUTING
	cv::GaussianBlur(I1, mu1, cv::Size(11, 11), 1.5);
	cv::GaussianBlur(I2, mu2, cv::Size(11, 11), 1.5);

	cv::Mat mu1_2 = mu1.mul(mu1);
	cv::Mat mu2_2 = mu2.mul(mu2);
	cv::Mat mu1_mu2 = mu1.mul(mu2);

	cv::Mat sigma1_2, sigma2_2, sigma12;

	cv::GaussianBlur(I1_2, sigma1_2, cv::Size(11, 11), 1.5);
	sigma1_2 -= mu1_2;

	cv::GaussianBlur(I2_2, sigma2_2, cv::Size(11, 11), 1.5);
	sigma2_2 -= mu2_2;

	cv::GaussianBlur(I1_I2, sigma12, cv::Size(11, 11), 1.5);
	sigma12 -= mu1_mu2;

	// Formular
	cv::Mat t1, t2, t3;

	t1 = 2 * mu1_mu2 + C1;
	t2 = 2 * sigma12 + C2;
	t3 = t1.mul(t2);                   // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

	t1 = mu1_2 + mu2_2 + C1;
	t2 = sigma1_2 + sigma2_2 + C2;
	t1 = t1.mul(t2);                   // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))

	cv::Mat ssim_map;
	divide(t3, t1, ssim_map);          // ssim_map =  t3./t1;

	cv::Scalar mssim = mean(ssim_map); // mssim = average of ssim map
	return mssim;
}


int main(void)
{
	
	cv::Mat img1 = cv::imread("D:\\Desktop\\aero1-1.tif", cv::IMREAD_GRAYSCALE);
	cv::Mat img2 = cv::imread("D:\\Desktop\\aero1-2.tif", cv::IMREAD_GRAYSCALE);

	int64 t1 = cv::getTickCount();
	cv::Scalar ssim = SSIM(img1, img2);
	int64 t2 = cv::getTickCount();

	std::cout << (double)(t2 - t1) / cv::getTickFrequency() << std::endl;

	// register
	cv::Ptr<cv::reg::MapperGradShift> mapper = cv::makePtr<cv::reg::MapperGradShift>();
	cv::reg::MapperPyramid mappPyr(mapper);
	cv::Ptr<cv::reg::Map> mapPtr = mappPyr.calculate(img1, img2);

	// print result
	cv::reg::MapShift* mapShift = dynamic_cast<cv::reg::MapShift*>(mapPtr.get());
	
	std::cout << std::endl << "--- Testing shift mapper ---" << std::endl;
	std::cout << cv::Mat(mapShift->getShift()) << std::endl;

	// display registration accuracy
	cv::Mat dest;
	mapShift->inverseWarp(img2, dest);
	//cv::imshow("RES", dest);

	showDifference(img1, dest, DIFF_REGPIX_IM);
	cv::waitKey(0);

	return 0;
}