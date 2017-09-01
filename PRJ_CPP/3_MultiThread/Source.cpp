/* Multi-thread demo */
#include <iostream>
#include <thread>
#include <string>

class ThreadTask
{
private:
	size_t count_ = 0;

public:
	explicit ThreadTask(size_t count) : count_(count)
	{

	}

	void operator()() const
	{

	}
};


class ThreadGuard
{
private:
	std::thread& t_;

public:
	explicit ThreadGuard(std::thread& t)
		: t_(t)
	{

	}

	~ThreadGuard()
	{
		if (this->t_.joinable())
		{
			this->t_.join();
		}
	}

	ThreadGuard(const ThreadGuard&) = delete;
	ThreadGuard& operator=(const ThreadGuard&) = delete;
};


void greeting()
{
	for (int i = 0; i < 1000; i++)
	{
		std::cout << "Hello multithread!" << std::endl;
	}
	return;
}


void greeting2(std::string string)
{
	for (int i = 0; i < 1000; i++)
	{
		std::cout << string << std::endl;
	}
	return;
}


void do_something()
{

}

void show()
{
	std::thread wk_thread;
	ThreadGuard g{ wk_thread };
	wk_thread = std::thread{ do_something };

	//do_something_in_current_thread();
	return;
}


int main(void)
{
	std::thread t( greeting );
	std::thread t2{ greeting2, "maobaoqi" };
	
	if (t.joinable())
	{
		t.join();
	}
	if (t2.joinable())
	{
		t2.join();
	}

	return 0;
}


