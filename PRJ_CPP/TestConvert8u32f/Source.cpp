#include <ippi.h>

#include <iostream>

int main(int argc, char** argv)
{
	IppiSize roi = { 5, 4 };

	Ipp32f img32f[15 * 20];
	Ipp8u img8u[5 * 4];

	ippiSet_32f_C1R(0.0f, img32f, 15 * sizeof(Ipp32f), {15, 20});
	ippiSet_8u_C1R(1, img8u, 5 * sizeof(Ipp8u), roi);
	img8u[5] = 10;
	img8u[6] = 10;

	ippiConvert_8u32f_C1R(img8u, 5 * sizeof(Ipp8u), img32f, 15 * sizeof(Ipp32f), roi);

	for (int i = 0; i < 15*20; i++)
	{
		std::cout << img32f[i] << " ";
		if ((i + 1) % 15 == 0)
		{
			std::cout << std::endl;
		}
	}
}