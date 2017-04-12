#include <iostream>

using namespace std;

const double PI = 3.1415926;

class Shapes
{
protected:
	int x, y;

public:
	void setValue(int d, int w = 0) { x = d, y = w; }
	virtual void disp() = 0;
};

class Square : public Shapes
{
public:
	void disp()
	{
		cout << "矩形面积：" << x*y << endl;
	}
};

class Circle : public Shapes
{
public:
	void disp()
	{
		cout << "圆形面积：" << PI*x*x << endl;
	}
};

int main()
{
	Shapes *ptr[2];
	Square s1;
	Circle c1;

	ptr[0] = &s1;
	ptr[0]->setValue(10, 5);
	ptr[0]->disp();

	ptr[1] = &c1;
	ptr[1]->setValue(10);
	ptr[1]->disp();

	return 0;
}