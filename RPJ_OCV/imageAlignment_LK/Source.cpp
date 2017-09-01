#include <imagealign\imagealign.h>

#include <opencv2\highgui.hpp>
#include <vector>

#include <opencv2\imgproc.hpp>
#include <opencv2\reg\mapshift.hpp>

namespace ia = imagealign;

// use a double precision warp description a similarity motion
// (rotation, translation and uniform scale)
typedef ia::WarpSimilarityD WarpType;

// use Inverse Composition algorithm for image alignment
typedef ia::AlignInverseCompositional<WarpType> AlignType;

int main(int argc, char** argv)
{
	// the template image
	cv::Mat tpl;
	tpl = cv::imread("D:\\Test Images\\Camera2\\test (1).tif", cv::IMREAD_GRAYSCALE);

	// the target image
	cv::Mat target;
	target = cv::imread("D:\\Test Images\\Camera2\\test (2).tif", cv::IMREAD_GRAYSCALE);

	// instance necessary objects
	WarpType w;
	std::vector<WarpType> incrementals;
	incrementals.push_back(w);
	AlignType a;
	a.prepare(tpl, target, w, 5);
	a.align(w, 10, 0.003, &incrementals);

	cv::Mat dst;
	for (int i = 0; i < incrementals.size(); i++)
	{
		ia::warpImage<uchar, ia::SAMPLE_BILINEAR>(target, dst, tpl.size(), incrementals[i]);
		target = dst;
	}

	cv::reg::

	return 0;
}