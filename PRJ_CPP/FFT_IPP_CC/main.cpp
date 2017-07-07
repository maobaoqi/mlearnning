//#include <ippi.h>
#include <iostream>

int main(void)
{
	unsigned char srcImg[1 * 1024];
	for (int i = 0; i < 1024; i++)
	{
		srcImg[i] = i;
	}
	
	std::cout << srcImg[100];

	return 0;
}