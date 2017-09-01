#include <iostream>


template<class T>
T my_add(T a, T b)
{
	return a + b;
}


template<class T>
T my_sum(T* p)
{
	double sum = 0.0;
	for (int i = 0; i < 5; i++)
	{
		sum += p[i];
	}

	return (T)sum;
}


int main(void)
{
	int int_a = 10, int_b = 5;
	std::cout << my_add(int_a, int_b) << std::endl;

	double d_a = 1.2, d_b = 1.3;
	std::cout << my_add(d_a, d_b) << std::endl;

	float num[5] = { 1.1f, 2.2f, 3.3f, 4.4f, 5.5f };
	float res = my_sum(num);
	std::cout << res << std::endl;

	int num_int[5] = { 1, 2, 3, 4, 5 };
	int res_int = my_sum(num_int);
	std::cout << res_int << std::endl;
}