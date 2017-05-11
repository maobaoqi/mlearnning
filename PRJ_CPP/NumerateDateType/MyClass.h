#ifndef MY_CLASS_H
#define MY_CLASS_H

class AddNum {

public:
	AddNum() {}
	AddNum(double a, double b) : num1(a), num2(b) {}

	double num1, num2;
	double getResult();

private:
	double res;
};
#endif
