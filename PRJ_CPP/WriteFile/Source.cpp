#include <fstream>
#include <iostream>
#include <string>

int main(void)
{
	std::string filename("D:/test.txt");
	std::ofstream fs(filename);
	if (!fs)
	{
		std::cout << "File cannot be opened!" << std::endl;
		return -1;
	}

	std::string content("maobaoqi");
	fs << content << std::endl;

	int value = 100;
	fs << value << std::endl;

	if (fs.is_open())
	{
		fs.close();
		std::cout << "File closed!" << std::endl;
	}

	return 0;
}