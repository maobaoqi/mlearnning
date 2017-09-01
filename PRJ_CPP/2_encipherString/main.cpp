#include <iostream>
#include <string>
#include <vector>

#include <map>

void SpeciyMacAddress(std::string mac)
{
	std::map<char, int> hexMap;
	hexMap =
	{
		{'0', 0},
		{'1', 1},
		{'2', 2},
		{'3', 3},
		{'4', 4},
		{'5', 5},
		{'6', 6},
		{'7', 7},
		{'8', 8},
		{'9', 9},
		{'A', 10},
		{'B', 11},
		{'C', 12},
		{'D', 13},
		{'E', 14},
		{'F', 15}
	};

	std::vector<std::string> macVec;
	std::vector<int> indexPos;
	auto iter = mac.cbegin();
	int index = 0;
	std::string temp;
	while (iter != mac.cend())
	{
		if (*iter == '-')
		{
			temp = mac.substr(index - 2, 2);
			macVec.push_back(temp);

		}
		index++;
		iter++;
	}
	temp = mac.substr(index - 2, index);
	macVec.push_back(temp);


	std::vector<int> macIntVec;
	for (int i = 0; i < macVec.size(); i++)
	{
		std::string singleStr = macVec.at(i);
		int intValue = 0;
		int powNum = (int)singleStr.size() - 1;

		for (int k = 0; k < singleStr.size(); k++)
		{
			auto it = hexMap.find(singleStr[k]);
			
			intValue += int((it->second)*pow(16, powNum));
			powNum -= 1;
		}

		macIntVec.push_back(intValue);
	}
}


int main(int argc, char** argv)
{
	std::string macAddr = "00-23-5a-15-92-5d";

	
	int vis[1001];
	int num = sizeof(vis);
	std::cout << num << std::endl;
	//SpeciyMacAddress(macAddr);
	
	
	return 0;
}