#include <ippi.h>
#include <iostream>
#include <deque>
#include <string>

#include <cmath>

class Demo {
public:
	Demo() : a(1), b(2)
	{

	}

	Demo& operator=(const Demo &demo)
	{
		if (this == &demo)
		{
			return *this;
		}
	}

	~Demo()
	{

	}



	int a;
	int b;
};


int main(void)
{
	//Ipp8u src1[8 * 4] = {
	//	100, 4, 2, 1, 0, 0, 0, 0,
	//	8, 4, 2, 1, 0, 0, 0, 0,
	//	8, 4, 2, 1, 0, 0, 0, 0,
	//	8, 4, 2, 1, 0, 0, 0, 0,
	//};
	//
	//Ipp8u src2[8 * 4] = {
	//	200, 3, 2, 1, 0, 0, 0, 0,
	//	4, 3, 2, 1, 0, 0, 0, 0,
	//	4, 3, 2, 1, 0, 0, 0, 0,
	//	4, 3, 2, 1, 0, 0, 0, 0,
	//};
	//
	//Ipp8u dst[8 * 4];
	//IppiSize srcROI = { 4, 4 };
	//int scaleFactor = 1;
	//
	//ippiAdd_8u_C1RSfs(src1, 8, src2, 8, dst, 4, srcROI, scaleFactor);
	//for (int i = 0; i < 16; i++)
	//{
	//	std::cout << int(dst[i]) << " " << std::endl;
	//}

	int length = 2050;
	int value = (int)floor(log2(length));
	
	std::cout << value << std::endl;

}