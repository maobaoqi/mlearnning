#pragma once
#include "BaseClass.h"
#include <string>

class PrivateChildClass :
	private BaseClass
{
public:
	PrivateChildClass();
	~PrivateChildClass();

public:
	std::string name;
};

