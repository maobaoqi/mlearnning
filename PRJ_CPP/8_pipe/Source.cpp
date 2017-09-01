#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <iostream>
#include <vector>
#include <fstream>

int main(void)
{
	FILE *fp;
	char Buf[200] = { 0 };

	std::remove("SysInfo.txt");

	// export os info
	if ((fp = _popen("wmic os >> SysInfo.txt", "r")) == nullptr)
	{
		perror("Fail to popen!\n");
		exit(1);
	}
	_pclose(fp);
	
	// export diskdrive info
	if ((fp = _popen("wmic diskdrive >> SysInfo.txt", "r")) == nullptr)
	{
		exit(1);
	}
	_pclose(fp);

		std::cout << "Well done!" << std::endl;
	getchar();

	return 0;

}