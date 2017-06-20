#include "opencv_dft_function.h"


using namespace cv;
using namespace std;

void testDFT()
{
	Mat img = imread("D:\\Desktop\\19380394_120511835000_2.jpg", IMREAD_GRAYSCALE);
	if (img.empty())
	{
		cout << "image load failed!" << endl;
		exit(0);
	}
	else
	{
		cout << "image load success!" << endl;
	}

	Mat padded;
	int m = getOptimalDFTSize(img.rows);
	int n = getOptimalDFTSize(img.cols);

	copyMakeBorder(img, padded, 0, m - img.rows, 0, n - img.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	Mat complexImg;
	merge(planes, 2, complexImg);

	dft(complexImg, complexImg);

	split(complexImg, planes);

	magnitude(planes[0], planes[1], planes[0]);
	Mat magI = planes[0];

	magI += Scalar::all(1);
	
	log(magI, magI);

	auto v1 = magI.rows & -2;
	auto v2 = magI.cols & -2;

	magI = magI(Rect(0, 0, img.cols, img.rows));

	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	Mat q0(magI, Rect(0, 0, cx, cy));
	Mat q1(magI, Rect(cx, 0, cx, cy));
	Mat q2(magI, Rect(0, cy, cx, cy));
	Mat q3(magI, Rect(cx, cy, cx, cy));

	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	normalize(magI, magI, 0, 1, CV_MINMAX);

	imshow("INPUT", img);
	imshow("OUTPUT", magI);

	waitKey(0);

}