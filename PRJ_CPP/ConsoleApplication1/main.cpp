// ptr
#include <iostream>
#include <memory>

using namespace std;


int main(int argc, char** agrv)
{

	allocator<string> alloc;
	auto const p = alloc.allocate(10);


	return 0;


}