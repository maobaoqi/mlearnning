#include <ippi.h>

#include <iostream>
#include <vector>

int main(void)
{
	Ipp32f* src = new Ipp32f[4 * 4];
	ippiSet_32f_C1R(0.0f, src, 4 * sizeof(Ipp32f), { 4, 4 });

	src[0] = 12.4f;
	src[1] = 22.2f;
	src[2] = 15.6f;

	Ipp64f meanValue;
	ippiMean_32f_C1R(src, 4 * sizeof(Ipp32f), { 4, 4 }, &meanValue, IppHintAlgorithm::ippAlgHintFast);

	std::cout << "Mean: " << meanValue << std::endl;

	Ipp32f* img_data;
	img_data = src;
	for (int i = 0; i < 4 * 4; i++)
	{
		std::cout << img_data[i] << " ";
		if ((i + 1) % 4 == 0)
		{
			std::cout << std::endl;
		}

		//img_data++;
	}

	std::cout << std::endl;

	Ipp8u* src2 = new Ipp8u[6 * 6];
	ippiSet_8u_C1R(0, src2, 6 * sizeof(Ipp8u), { 6, 6 });

	ippiSet_8u_C1R(5, src2 + 2 + 2 * 6 * sizeof(Ipp8u), 6*sizeof(Ipp8u), { 2, 2 });

	for (int i = 0; i < 6 * 6; i++)
	{
		std::cout << int(src2[i]) << " ";
		if ((i + 1) % 6 == 0)
		{
			std::cout << std::endl;
		}
	}

	std::vector<int> numVec;
	for (int i = 0; i < 10; i++)
	{
		numVec.push_back(i);
	}

	numVec.clear();

	for (int i = 0; i < 5; i++)
	{
		numVec.push_back(i * 10);
	}

	auto iter = numVec.cbegin();
	while (iter != numVec.cend())
	{
		std::cout << *iter++ << " ";
	}

	std::cout << std::endl;


	delete[] src;
	delete[] src2;
	return 0;
}