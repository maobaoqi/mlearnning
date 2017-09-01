// MacAddress.cpp : Defines the entry point for the console application.
//

#include "KeyClass.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
	
	KeyClass deviceKey;
	deviceKey.SpecifyMacAddress("08-00-27-8D-50-7A");
	deviceKey.ShowMacAddress();
	
	std::string key;
	key = deviceKey.GenerateKey();
	std::cout << "Key: " << key << std::endl;

	bool res = deviceKey.CheckKey(key);

	if (res == true)
	{
		std::cout << "Pass!" << std::endl;
	}
	else
	{
		std::cout << "Failed!" << std::endl;
	}

	return 0;
}