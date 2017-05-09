#include <iostream>

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

	return 0;
}