#include <fstream>
#include <iostream>
#include <vector>
#include <io.h>
#include <string>
#include <stdlib.h>

using namespace std;

void getFiles(string path, string extendName, vector<string>& fileNames)
{
	long hFile = 0;
	struct _finddata_t fileInfo;
	string pathName, exdName;

	if (0 != strcmp(extendName.c_str(), ""))
	{
		exdName = "\\*." + extendName;
	}
	else
	{
		exdName = "\\*";
	}

	if ((hFile = _findfirst(pathName.assign(path).append(exdName).c_str(), &fileInfo)) != -1)
	{
		do
		{
			if (fileInfo.attrib & _A_SUBDIR)
			{
				if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)
				{
					getFiles(pathName.assign(path).append("\\").append(fileInfo.name), extendName, fileNames);
				}
			}
			else
			{
				if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)
				{
					fileNames.push_back(pathName.assign(path).append("\\").append(fileInfo.name));
				}
			}
		} while (_findnext(hFile, &fileInfo) == 0);

		
		_findclose(hFile);
	}
}

int main(void)
{
	//char* filePath = "D:\\New folder\\0-2277";
	//vector<string> files;
	//
	//getFiles(filePath, "tif", files);
	//
	//for (auto str : files)
	//{
	//	cout << str.c_str() << endl;
	//}

	int i = 0;
	std::vector<std::string> strVec;
	for (int i = 0; i < 5; i++)
	{
		char name[30];
		sprintf_s(name, "D:\\result_%d", i);
		std::string filename(name);
		strVec.push_back(filename);
	}

	for (auto str : strVec)
	{
		std::cout << str << std::endl;
	}
}