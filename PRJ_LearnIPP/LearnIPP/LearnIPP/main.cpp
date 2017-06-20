#include "ippi.h"
#include "ippcore.h"

#include <iostream>

void statusInfo(void)
{
	IppiSize roi = { 0 };
	IppStatus status = ippiSet_8u_C1R(3, 0, 0, roi);
	printf("%d : %s\n", status, ippGetStatusString(status));
}

int main(int argc, char** argv)
{
	const IppLibraryVersion* ippVersion = ippiGetLibVersion();

	statusInfo();

	return 0;
}