#ifndef HARD_DISK_SERIAL_NUM_H
#define HARD_DISK_SERIAL_NUM_H

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>


class HardDiskSN
{
public:
	HardDiskSN()
	{

	}


	~HardDiskSN()
	{

	}

	
	bool GetAllHardDiskSN(std::vector<std::string>& hdsns)
	{
		std::string command("wmic diskdrive get serialnumber");
		std::string result = exeCommand(command);
	}


	bool GetFirstHardDiskSN(std::string& hdsn)
	{
		std::string command("wmic diskdrive where index=0 get serialnumber");
		hdsn = exeCommand(command);
		size_t index_n1 = hdsn.find_first_of('\n');
		size_t index_n2 = 0;
		for (size_t i = index_n1; i < hdsn.length(); i++)
		{
			if (hdsn[i] == ' ')
			{
				index_n2 = i;
				break;
			}
			if (hdsn[i] == '\r')
			{
				index_n2 = i - 1;
				break;
			}
		}
		hdsn = hdsn.substr(index_n1 + 1, index_n2 - index_n1);

		if (hdsn.empty())
		{
			return false;
		}

		return true;
	}


	bool GetHardDiskSN(std::string& hdsn, int index)
	{

	}


private:

	std::string exeCommand(const std::string& command)
	{
		std::array<char, 128> buffer;
		std::string result;
		std::shared_ptr<FILE> pipe(_popen(command.c_str(), "r"), _pclose);
		if (!pipe)
		{
			throw std::runtime_error("_popen() failed!");
		}
		
		while (!feof(pipe.get())) 
		{
			if (fgets(buffer.data(), 128, pipe.get()) != NULL)
			{
				result += buffer.data();
			}
		}
		return result;
	}
};

#endif