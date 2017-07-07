// map instead switch
#include <map>
#include <iostream>
#include <string>

#include <set>
#include <memory>

enum EnumType
{
	one,
	two,
	three
};

void showMessageBox()
{
	std::cout << "The value is:" << std::endl;
}

void showMessageBox2(double value)
{
	std::cout << "The double value is:" << value << std::endl;
}

void ShowMessageBox3(std::string str)
{
	std::cout << "The double value is:" << std::endl;
}

int main(void)
{
	//typedef void(*func)();
	//
	//std::map<EnumType, func> funcMap;
	//funcMap.insert({one, showMessageBox});
	////funcMap.insert(std::make_pair(two, showMessageBox2(double value)));
	//
	//std::set<std::string> myset = { "the", "and", "The", "we" };
	//std::map<std::string, int> mymap;
	//
	//std::string word;
	//while (std::cin >> word)
	//{
	//	if (myset.find(word) == myset.cend())
	//	{
	//		mymap[word]++;
	//	}
	//}

	int value;
	value = 1 << 8;
    std::cout << value << std::endl;

	unsigned int *src = new unsigned int[128 * 128];
	memset(src, 0, 128 * 128 * sizeof(unsigned int));

	std::cout << sizeof(*src) << std::endl;

	return 0;
}