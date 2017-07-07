#include <ippi.h>
#include <iostream>

int main(void)
{
	int stepBytes;
	Ipp32f *src = new Ipp32f[6 * 6];
	

	ippiSet_32f_C1R(2.1f, src, 6*sizeof(Ipp32f), { 6, 6 });

	src[0] = 1.0f;
	src[6 - 1] = 1.0f;
	src[5 * 6] = 1.0f;
	src[5 * 6 + 5] = 1.0f;
	
	for (int i = 0; i < 36; i++)
	{
		std::cout << float(src[i]) << " ";
		if ((i + 1) % 6 == 0)
		{
			std::cout << std::endl;
		}
	}

	std::cout << std::endl;

	int step;
	Ipp32f *dst = new Ipp32f[12 * 12];
	ippiSet_32f_C1R(0.0f, dst, 12*sizeof(Ipp32f), { 12, 12 });
	
	
	ippiCopy_32f_C1R(src, 6*sizeof(Ipp32f), dst, 12*sizeof(Ipp32f), { 3, 3 });
	
	ippiCopy_32f_C1R(src + 3*sizeof(Ipp32f),
		6 * sizeof(Ipp32f),
		dst + (12 - 3)*sizeof(Ipp32f),
		12 * sizeof(Ipp32f),
		{ 3, 3 });
	
	//ippiCopy_8u_C1R(src + (6*3 + 3)*sizeof(Ipp8u),
	//	stepBytes,
	//	dst + 12*6,
	//	step,
	//	{ 3, 3 });
	
	for (int i = 0; i < 144; i++)
	{
		std::cout << float(dst[i]) << " ";
		if ((i + 1) % 12 == 0)
		{
			std::cout << std::endl;
		}
	}

	delete[] src;
	delete[] dst;

	return 0;
}