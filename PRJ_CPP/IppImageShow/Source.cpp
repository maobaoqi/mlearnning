#include <ipp.h>

#include <memory>

#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>

#include <ippi.h>

int main(void)
{
	Ipp8u *img = new Ipp8u[512 * 512];
	//std::shared_ptr<Ipp8u> img = std::make_shared<Ipp8u>(new Ipp8u[64 * 64]);
	ippiSet_8u_C1R(100, img, 512 * sizeof(Ipp8u), { 512, 512 });

	cv::Mat matSrc;
	matSrc.create(cv::Size(512, 512), CV_8U);
	cv::imshow("Src", matSrc);
	
	int size;
	ippiWinHammingGetBufferSize(IppDataType::ipp8u, { 512, 512 }, &size);
	Ipp8u* buffer = (Ipp8u*)ippMalloc(size);

	ippiWinHamming_8u_C1R(img, 512 * sizeof(Ipp8u), (Ipp8u*)&matSrc.data[0], 512 * sizeof(Ipp8u), { 512, 512 }, buffer);

	cv::imshow("Src", matSrc);

	cv::waitKey(0);

	delete[] img;
	return 0;
}