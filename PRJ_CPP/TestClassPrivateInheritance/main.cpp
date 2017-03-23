#include "PrivateChildClass.h"
#include <iostream>

int main(int argc, char** argv)
{
	PrivateChildClass aPrivateChild;
	
	std::cout << aPrivateChild.name << std::endl;

	return 0;
}