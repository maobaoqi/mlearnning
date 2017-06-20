#pragma once
class Base {
public:
	int pubVal = 10;
	
protected:
	int proVal = 20;
	int CalSum();

private:
	int priVal = 30;
};