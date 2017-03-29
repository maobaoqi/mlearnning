#include <opencv2\imgcodecs.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <iostream>

using namespace cv;

double alpha;
double beta;

int main(int argc, char** argv)
{
	Mat image = imread("C:\\Users\\benjy.mao\\Desktop\\timg.jpg");
	Mat new_image = Mat::zeros(image.size(), image.type());

	std::cout << "Basic Linear Transforms" << std::endl;
	std::cout << "-----------------------" << std::endl;
	std::cout << "Enter the alpha value [1.0-3.0]";
	std::cin >> alpha;
	std::cout << "Enter the beta value[0-100]:";
	std::cin >> beta;

	int64 t1 = getTickCount();
	image.convertTo(new_image, -1, alpha, beta);
	std::cout << (getTickCount() - t1) / getTickFrequency() << std::endl;

	int64 t2 = getTickCount();
	for (int y = 0; y < image.rows; y++)
	{
		for (int x = 0; x < image.cols; x++)
		{
			for (int c = 0; c < 3; c++)
			{
				new_image.at<cv::Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha*(image.at<cv::Vec3b>(y, x)[c]) + beta);
			}
		}
	}
	std::cout << (getTickCount() - t2) / getTickFrequency() << std::endl;

	namedWindow("Original Image", 1);
	namedWindow("New Image", 1);

	imshow("Original Image", image);
	imshow("New Image", new_image);

	waitKey(0);
	return 0;
}