#include "stdio.h"
#include <opencv\cv.h>
#include <opencv\cxcore.h>
#include <opencv\highgui.h> 
#include <opencv\cvaux.h> 

#include <opencv2\highgui.hpp>

#define MAX_MOTION 
#define BLOCK_WIDTH 16
#define BLOCK_HEIGHT 16


double SAD(IplImage *im_left, IplImage *im_right, int mvx, int mvy, int height, int width, int mv_up)
{

	double sad = 0;

	if (mv_up)
	{
		for (int i = 0; i<height; i++)
			for (int j = 0; j<width; j++)
				sad += abs(*(im_left->imageData + im_left->width*(im_left->height / 2 - height / 2 + i) + mvx + j) -
					*(im_right->imageData + im_right->width*(im_right->height / 2 - height / 2 + i + mvy)));
	}
	else
	{
		for (int i = 0; i<height; i++)
			for (int j = 0; j<width; j++)
				sad += abs(*(im_left->imageData + im_left->width*(im_left->height / 2 - height / 2 + i) + mvx + j) -
					*(im_right->imageData + im_right->width*(im_right->height / 2 - height / 2 + i - mvy)));
	}
	return sad;
}

int main(int argc, char ** argv)
{
	IplImage * im_left;
	IplImage * im_right;
	IplImage * im_parse;

	IplImage * realInput1;
	IplImage * imaginaryInput1;
	IplImage * complexInput1;
	IplImage * realInput2;
	IplImage * imaginaryInput2;
	IplImage * complexInput2;
	IplImage * dft_E;

	IplImage * image_Re;
	IplImage * image_Im;
	IplImage * image_parse;
	IplImage * rightToleft;
	IplImage * im_diff;
	int mvx, mvy, mv_up, dx, dy;
	CvMat *dst_image;
	int m, n;

	//im_left = cvLoadImage("4.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat leftImg = cv::imread("D:\\git\\mlearnning\\PRJ_CPP\\x64\\Debug\\4.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat rightImg = cv::imread("D:\\git\\mlearnning\\PRJ_CPP\\x64\\Debug\\5.bmp", CV_LOAD_IMAGE_GRAYSCALE);

	cv::imshow("1", leftImg);
	cv::imshow("2", rightImg);

	im_left = &IplImage(leftImg);
	im_right = &IplImage(rightImg);

	//im_right = cvLoadImage("5.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	if (!im_right || !im_left)
		return -1;
	cvNamedWindow("win1");
	cvShowImage("win1", im_left);
	cvNamedWindow("win2");
	cvShowImage("win2", im_right);

	int width = im_left->width;
	int height = im_left->height;
	CvSize im_size = cvGetSize(im_left);
	image_Re = cvCreateImage(im_size, IPL_DEPTH_64F, 1);
	image_Im = cvCreateImage(im_size, IPL_DEPTH_64F, 1);
	image_parse = cvCreateImage(im_size, IPL_DEPTH_64F, 1);
	rightToleft = cvCreateImage(im_size, IPL_DEPTH_8U, 1);
	im_diff = cvCreateImage(im_size, IPL_DEPTH_8U, 1);
	dst_image = cvCreateMat(height, width, CV_8UC1);

	realInput1 = cvCreateImage(im_size, IPL_DEPTH_64F, 1);
	imaginaryInput1 = cvCreateImage(im_size, IPL_DEPTH_64F, 1);
	complexInput1 = cvCreateImage(im_size, IPL_DEPTH_64F, 2);

	dft_E = cvCreateImage(im_size, IPL_DEPTH_64F, 2);
	//im_left DFT
	cvScale(im_left, realInput1, 1.0, 0.0);
	cvZero(imaginaryInput1);
	cvMerge(realInput1, imaginaryInput1, NULL, NULL, complexInput1);
	cvDFT(complexInput1, complexInput1, CV_DXT_FORWARD);

	realInput2 = cvCreateImage(im_size, IPL_DEPTH_64F, 1);
	imaginaryInput2 = cvCreateImage(im_size, IPL_DEPTH_64F, 1);
	complexInput2 = cvCreateImage(im_size, IPL_DEPTH_64F, 2);
	//im_right DFT
	cvScale(im_right, realInput2, 1.0, 0.0);
	cvZero(imaginaryInput2);
	cvMerge(realInput2, imaginaryInput2, NULL, NULL, complexInput2);
	cvDFT(complexInput2, complexInput2, CV_DXT_FORWARD);
	
	cvMulSpectrums(complexInput1, complexInput2, dft_E, CV_DXT_MUL_CONJ);
	cvSplit(dft_E, image_Re, image_Im, NULL, NULL);

	// Compute the magnitude of the spectrum Mag = sqrt(Re^2 + Im^2)
	cvPow(image_Re, image_Re, 2.0);
	cvPow(image_Im, image_Im, 2.0);
	cvAdd(image_Re, image_Im, image_parse, NULL);
	cvPow(image_parse, image_parse, 0.5);

	cvSplit(dft_E, image_Re, image_Im, 0, 0);
	cvDiv(image_Re, image_parse, image_Re);
	cvDiv(image_Im, image_parse, image_Im);
	cvMerge(image_Re, image_Im, NULL, NULL, dft_E);
	cvDFT(dft_E, dft_E, CV_DXT_INVERSE);

	// Compute the magnitude of the spectrum Mag = sqrt(Re^2 + Im^2)
	cvSplit(dft_E, image_Re, image_Im, 0, 0);
	cvPow(image_Re, image_Re, 2.0);
	cvPow(image_Im, image_Im, 2.0);
	cvAdd(image_Re, image_Im, image_Re, NULL);
	cvPow(image_Re, image_Re, 0.5);
	CvPoint min_loc = cvPoint(0, 0);
	CvPoint max_loc = cvPoint(0, 0);
	double min_val;
	double max_val;
	cvMinMaxLoc(image_Re, &min_val, &max_val, &min_loc, &max_loc, NULL);


	//	cvCvtScale(im_right,dst_image);
	//	cvSaveImage("555.bmp", dst_image );
	//	cvCvtScale(im_left,dst_image);
	//	cvSaveImage("666.bmp", dst_image );

	mvx = max_loc.x;
	mvy = max_loc.y;
	if (mvy>height / 2)
		mvy = height - mvy;
	if (SAD(im_left, im_right, mvx, mvy, BLOCK_WIDTH, BLOCK_HEIGHT, 1)>SAD(im_left, im_right, mvx, mvy, BLOCK_WIDTH, BLOCK_HEIGHT, 0))
	{
		mv_up = 0;
	}
	else
	{
		mv_up = 1;
	}

	//	if(SAD())
	double leftsum, rightsum, ratio;
	leftsum = 0;
	rightsum = 0;
	ratio = 0;
	while (fabs(ratio - 1)>0.0005)
	{
		for (m = 0; m<height - mvy; m++)
			for (n = 0; n<width - mvx; n++)
			{
				leftsum += (unsigned char)*(im_left->imageData + (mvy + m)*width + n + mvx);
				rightsum += (unsigned char)*(im_right->imageData + m*width + n);
			}
		ratio = leftsum / rightsum;
		cvScale(im_right, im_right, ratio);
		leftsum = 0;
		rightsum = 0;

	}
	dx = mvx;
	dy = mvy;
	while (SAD(im_left, im_right, dx + 1, mvy, BLOCK_WIDTH, BLOCK_HEIGHT, 0) <= SAD(im_left, im_right, dx, mvy, BLOCK_WIDTH, BLOCK_HEIGHT, 0))
	{
		dx++;
	}
	while (SAD(im_left, im_right, dx - 1, mvy, BLOCK_WIDTH, BLOCK_HEIGHT, 0) <= SAD(im_left, im_right, dx, mvy, BLOCK_WIDTH, BLOCK_HEIGHT, 0))
	{
		dx--;
	}
	while (SAD(im_left, im_right, mvx, dy + 1, BLOCK_WIDTH, BLOCK_HEIGHT, 0) <= SAD(im_left, im_right, mvx, dy, BLOCK_WIDTH, BLOCK_HEIGHT, 0))
	{
		dy++;
	}
	while (SAD(im_left, im_right, mvx, mvy - 1, BLOCK_WIDTH, BLOCK_HEIGHT, 0) <= SAD(im_left, im_right, mvx, mvy, BLOCK_WIDTH, BLOCK_HEIGHT, 0))
	{
		mvy--;
	}
	cvCopy(im_left, rightToleft, NULL);
	for (int i = 0; i<height - mvy; i++)
		for (int j = 0; j<width - mvx; j++)
		{
			*(rightToleft->imageData + width*(i + mvy) + mvx + j) = *(im_right->imageData + width*i + j);
		}
	cvAbsDiff(im_left, rightToleft, im_diff);

	im_parse = cvCreateImage(cvSize(width + mvx, height - mvy), IPL_DEPTH_8U, 1);
	im_parse->widthStep = width + mvx;

	for (m = 0; m<height - mvy; m++)
		for (n = 0; n<mvx; n++)
		{
			*(im_parse->imageData + m*(width + mvx) + n) = *(im_left->imageData + mvy*width + m*width + n);
		}

	for (m = 0; m<height - mvy; m++)
		for (n = 0; n<width; n++)
		{
			*(im_parse->imageData + m*(width + mvx) + n + mvx) = *(im_right->imageData + m*width + n);
		}

	cvNamedWindow("win3");
	cvShowImage("win3", im_parse);
	cvNamedWindow("win4");
	cvShowImage("win4", rightToleft);
	cvWaitKey(-1);
	return 0;
}