#include <list>
#include <vector>
#include <ctime>
#include <iostream>
#include <random>

int main(void)
{
	std::vector<int> intVector;
	std::list<int> intList;
	for (int i = 0; i < 8000; i++)
	{
		intVector.push_back(i);
		intList.push_back(i);
	}

	// compare earse performance for vector and list
	clock_t t1, t2;
	t1 = clock();
	while (!intVector.empty())
	{
		auto iter = intVector.begin();
		intVector.erase(iter);
	}
	t2 = clock();
	std::cout << (double)(t2 - t1) / CLOCKS_PER_SEC * 1000 << std::endl;

	t1 = clock();
	while (!intList.empty())
	{
		auto iter = intList.begin();
		intList.erase(iter);
	}
	t2 = clock();
	std::cout << (double)(t2 - t1) / CLOCKS_PER_SEC * 1000 << std::endl;

	int intRand = std::rand();
	std::cout << intRand << std::endl;

    std::srand(time(nullptr));
	std::cout << std::rand() << std::endl;
	
	srand(time(nullptr));
	int num[10];
	for (int i = 0; i < 10; i++)
	{
		num[i] = rand();
		std::cout << num[i] << " ";
	}
	std::cout << std::endl;

	return 0;
}