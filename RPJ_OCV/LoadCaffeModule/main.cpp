#include <opencv2\dnn.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>

using namespace cv;
using namespace cv::dnn;

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

/* find the best class for the blob */
void getMaxClass(dnn::Blob &probBlob, int *classId, double *classProb)
{
	// reshape the blob to 1*1000 matrix
	Mat probMax = probBlob.matRefConst().reshape(1, 1);
	Point classNumber;

	minMaxLoc(probMax, nullptr, classProb, nullptr, &classNumber);
	*classId = classNumber.x;
}

std::vector<string> readClassNames(const char *filename = "E:\\Documents\\Visual Studio 2015\\Projects\\mlearnning\\RPJ_OCV\\x64\\Debug\\synset_words.txt")
{
	std::vector<string> classNames;

	std::ifstream fp(filename);
	if (!fp.is_open())
	{
		std::cerr << "File with classes labels not found:" << filename << std::endl;
		exit(-1);
	}

	std::string name;
	while (!fp.eof())
	{
		std::getline(fp, name);
		if (name.length())
		{
			classNames.push_back(name.substr(name.find(' ') + 1));
		}
	}

	fp.close();
	return classNames;
}

int main(int argc, char** argv)
{
	// Required if OpenCV is built as static libs.
	cv::dnn::initModule();

	String modelTxt = "E:\\Documents\\Visual Studio 2015\\Projects\\mlearnning\\RPJ_OCV\\x64\\Debug\\bvlc_googlenet.prototxt";
	String modelBin = "E:\\Documents\\Visual Studio 2015\\Projects\\mlearnning\\RPJ_OCV\\x64\\Debug\\bvlc_googlenet.caffemodel";
	String imageFile = "E:\\Documents\\Visual Studio 2015\\Projects\\mlearnning\\RPJ_OCV\\x64\\Debug\\Koala.jpg";

	// Get dnn net.
	Net net = dnn::readNetFromCaffe(modelTxt, modelBin);
	if (net.empty())
	{
		std::cerr << "Can't load network by using the following files:" << std::endl;
		std::cerr << "prototxt:" << modelTxt << std::endl;
		std::cerr << "caffemodel:" << modelBin << std::endl;
		std::cerr << "bvlc_googlenet.caffemodel can be downloaded here:" << std::endl;
		exit(-1);
	}

	Mat img = imread(imageFile);
	if (img.empty())
	{
		std::cerr << "Can't read image from the file:" << imageFile << std::endl;
		exit(-1);
	}

	// GoogleNet accepts only 244*244 RGB-images
	resize(img, img, Size(224, 224));
	cvtColor(img, img, cv::COLOR_BGR2RGB);

	// Convert Mat to dnn::Blob batch of images.
	dnn::Blob inputBlob = dnn::Blob::fromImages(img);
	net.setBlob(".data", inputBlob);
	net.forward();
	dnn::Blob prob = net.getBlob("prob");

	int classId;
	double classProb;
	getMaxClass(prob, &classId, &classProb);

	std::vector<string> classNames = readClassNames();
	std::cout << "Best class:#" << classId << "'" << classNames.at(classId) << std::endl;
	std::cout << "Probability:" << classProb * 100 << "%" << std::endl;

	waitKey(0);
	return 0;
}