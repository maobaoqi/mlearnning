#include <iostream>

#include "MyClass.h"

enum Finger
{
	DMZ = 0,
	SZ = 1,
	ZZ = 2,
	WMZ = 3,
	XMZ = 4
};


int main(int argc, char** argv)
{
	Finger thisThumb = DMZ;
	std::cout << "this is a thumb:" << thisThumb << std::endl;

	int val1 = static_cast<int>(7.9);
	double val2 = static_cast<double>(25);

	std::cout << val1 << std::endl;
	std::cout << val2 << std::endl;

	AddNum addOperator;
	addOperator.num1 = 1.2;
	addOperator.num2 = 1.7;
	std::cout << addOperator.getResult() << std::endl;

	return 0;
}