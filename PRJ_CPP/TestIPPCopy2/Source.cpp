#include <ippi.h>

#include <iostream>

int main(int argc, char* argv[])
{

	Ipp32f* src = new Ipp32f[8 * 8];
	int srcStride = 8 * sizeof(Ipp32f);
	IppiSize srcROI = { 8, 8 };
	ippiSet_32f_C1R(1.0f, src, srcStride, srcROI);

	Ipp32f* dst = new Ipp32f[8 * 8];
	ippiSet_32f_C1R(2.0f, dst, srcStride, srcROI);

	IppiSize roi = { 6, 6 };
	
	ippiCopy_32f_C1R
	(
		src, 
		srcStride, 
		dst + 8 * 2 + 2, 
		//dst,
		srcStride, 
		roi
	);

	for (int i = 0; i < 64; i++)
	{
		std::cout << (float)dst[i] << " ";
		if ((i + 1) % 8 == 0)
		{
			std::cout << std::endl;
		}
	}
	
	delete[] src;
	delete[] dst;
	
	return 0;
}