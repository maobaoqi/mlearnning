#include <Windows.h>
#include <wincon.h>
#include <stdlib.h>
#include <stdio.h>
#include <nb30.h>
#include <vector>
#include <string>
#include <map>

#pragma comment(lib, "netapi32.lib")

class KeyClass
{
public:

	KeyClass()
		: m_macAddress(nullptr),
		m_keySeedStr("BlueJayImaging"),
		m_isLocalDeviceMacAddress(true)
	{
		m_macAddress = new char[200];
	}


	~KeyClass()
	{
		delete[] m_macAddress;
	}


	std::string GenerateKey()
	{
		if (m_isLocalDeviceMacAddress)
		{
			GetLocalDeviceMacAddress();
		}
		
		std::vector<KeyCode> output;
		EncipheringKey(output);

		std::vector<int> keyInt;
		for (int i = 0; i < output.size(); i++)
		{
			keyInt.push_back(output[i].firstPart);
			keyInt.push_back(output[i].secondPart);
		}

		std::string key;
		for (int i = 0; i < keyInt.size(); )
		{
			char ch1[8];
			char ch2[8];
			_itoa(keyInt[i], ch1, 10);
			_itoa(keyInt[i + 1], ch2, 10);
			if (i + 2 < keyInt.size())
			{
				std::string str1(ch1);
				std::string str2(ch2);
				if (str2.length() == 1)
				{
					str2 = "0" + str2;
				}
				key = key + str1 + str2 + "-";
			}
			else
			{
				std::string str1(ch1);
				std::string str2(ch2);
				if (str2.length() == 1)
				{
					str2 = "0" + str2;
				}
				key = key + str1 + str2;
			}

			i += 2;
		}

		return key;
	}


	bool CheckKey(std::string key)
	{
		std::vector<KeyCode> inputKey;

		if (key.empty())
		{
			return false;
		}

		auto iter = key.cbegin();
		int idx_s = 0, idx_e = 0;
		std::vector<std::string> strVec;
		std::string subStr;
		while (iter != key.cend())
		{
			//int singleVal, first, second;
			if (*iter == '-')
			{
				subStr = key.substr(idx_s, (idx_e - idx_s));
				idx_s = idx_e + 1;
				strVec.push_back(subStr);
				//singleVal = atoi(subStr.c_str());
			}

			idx_e++;
			iter++;
		}
		strVec.push_back(key.substr(key.length() - 4, 4));

		KeyCode keySlice;
		for (int i = 0; i < strVec.size(); i++)
		{
			int val1, val2;
			int tmp = atoi(strVec[i].c_str());

			val1 = tmp / 100;
			val2 = tmp % 100;

			keySlice.firstPart = val1;
			keySlice.secondPart = val2;

			inputKey.push_back(keySlice);
		}

		std::vector<int> calcMacAddress;
		DecipheringKey(inputKey, calcMacAddress);

		// check
		if (m_isLocalDeviceMacAddress)
		{
			GetLocalDeviceMacAddress();
		}
		bool checkPass = true;
		if (m_macAddressVec.size() == calcMacAddress.size())
		{
			for (int i = 0; i < m_macAddressVec.size(); i++)
			{
				if (m_macAddressVec[i] != calcMacAddress[i])
				{
					checkPass = false;
				}
			}
		}
		else
		{
			checkPass = false;
		}


		return checkPass;
	}


	void SetKeySeed(std::string seed)
	{
		m_keySeedStr = seed;
	}


	void SpecifyMacAddress(const std::string macAddr)
	{
		std::string mac = macAddr;

		m_isLocalDeviceMacAddress = false;
		if (!m_macAddressVec.empty())
		{
			m_macAddressVec.clear();
		}


		std::map<char, int> hexMap;
		hexMap =
		{
			{ '0', 0 },
			{ '1', 1 },
			{ '2', 2 },
			{ '3', 3 },
			{ '4', 4 },
			{ '5', 5 },
			{ '6', 6 },
			{ '7', 7 },
			{ '8', 8 },
			{ '9', 9 },
			{ 'A', 10 },
			{ 'B', 11 },
			{ 'C', 12 },
			{ 'D', 13 },
			{ 'E', 14 },
			{ 'F', 15 }
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

			m_macAddressVec.push_back(intValue);
		}
	}


	void ShowMacAddress()
	{
		if (m_macAddressVec.empty())
		{
			GetLocalDeviceMacAddress();
			PrintMacAddress();
		}
		else
		{
			PrintMacAddress();
		}
	}


private:

	char* m_macAddress;
	std::vector<int> m_macAddressVec;
	std::string m_keySeedStr;

	bool m_isLocalDeviceMacAddress;

	struct KeyCode
	{
		int firstPart;
		int secondPart;
	};

	
	int GetLocalDeviceMacAddress()
	{
		NCB ncb;

		struct ASTAT
		{
			ADAPTER_STATUS adapt;
			NAME_BUFFER NameBuff[30];
		};

		ASTAT Adapter;

		struct LANA_ENUM
		{
			UCHAR length;
			UCHAR lana[MAX_LANA];
		};

		LANA_ENUM lana_enum;
		UCHAR uRetCode;
		memset(&ncb, 0, sizeof(ncb));
		memset(&lana_enum, 0, sizeof(lana_enum));
		ncb.ncb_command = NCBENUM;
		ncb.ncb_buffer = (unsigned char *)&lana_enum;
		ncb.ncb_length = sizeof(LANA_ENUM);
		uRetCode = Netbios(&ncb);

		if (uRetCode != NRC_GOODRET)
			return uRetCode;

		for (int lana = 0; lana<lana_enum.length; lana++)
		{
			ncb.ncb_command = NCBRESET;
			ncb.ncb_lana_num = lana_enum.lana[lana];
			uRetCode = Netbios(&ncb);
			if (uRetCode == NRC_GOODRET)
				break;
		}

		if (uRetCode != NRC_GOODRET)
			return uRetCode;

		memset(&ncb, 0, sizeof(ncb));
		ncb.ncb_command = NCBASTAT;
		ncb.ncb_lana_num = lana_enum.lana[0];
		strcpy((char*)ncb.ncb_callname, "*");
		ncb.ncb_buffer = (unsigned char *)&Adapter;
		ncb.ncb_length = sizeof(Adapter);
		uRetCode = Netbios(&ncb);

		if (uRetCode != NRC_GOODRET)
			return uRetCode;

		
		m_macAddressVec.clear();
		for (int i = 0; i < 6; i++)
		{
			int temp = int(Adapter.adapt.adapter_address[i]);
			m_macAddressVec.push_back(temp);
		}

		m_isLocalDeviceMacAddress = true;
		return 0;
	}


	int EncipheringKey(std::vector<KeyCode> &outputKey)
	{
		int seed = 0;
		for (int i = 0; i < m_keySeedStr.length(); i++)
		{
			seed += (int)m_keySeedStr[i];
		}

		auto iter = m_macAddressVec.cbegin();
		while (iter != m_macAddressVec.cend())
		{
			int temp = (*iter) + seed;

			int val1 = temp / 100;
			int val2 = temp % 100;

			KeyCode keySlice;
			keySlice.firstPart = val1;
			keySlice.secondPart = val2;

			outputKey.push_back(keySlice);
			iter++;
		}

		return 0;
	}


	int DecipheringKey(std::vector<KeyCode> inputKey, std::vector<int>& macVec)
	{
		int seed = 0;
		for (int i = 0; i < m_keySeedStr.length(); i++)
		{
			seed += (int)m_keySeedStr[i];
		}

		auto iter = inputKey.cbegin();
		while (iter != inputKey.cend())
		{
			int firstChar = iter->firstPart;
			int secondChar = iter->secondPart;

			firstChar *= 100;

			int value = firstChar + secondChar;
			value -= seed;

			macVec.push_back(value);

			iter++;
		}

		return 0;
	}


	void PrintMacAddress()
	{
		char macAddress[200];
		sprintf(macAddress, "%02X-%02X-%02X-%02X-%02X-%02X",
			m_macAddressVec[0],
			m_macAddressVec[1],
			m_macAddressVec[2],
			m_macAddressVec[3],
			m_macAddressVec[4],
		    m_macAddressVec[5]);


		printf(macAddress);
		printf("\n");
	}
};