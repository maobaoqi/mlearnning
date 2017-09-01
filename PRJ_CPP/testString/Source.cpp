#include <string>
#include <iostream>

int main(void)
{
	std::string fileAdd("D:\\Desktop\\Key tool\\license_user.key");

	size_t len = fileAdd.length();
	size_t res = fileAdd.find_last_of(".key");

	std::string fileAdd2("D:\\Desktop\\Key tool\\license_user.kes");
	size_t res2 = fileAdd2.find_last_of(".key");

	return 0;
}