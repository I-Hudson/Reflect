#pragma once

#include "Reflect.h"
#include "Generated/TestStrcuts_reflect_generated.h"
#include <iostream>

REFLECT_CLASS(Person, Public, Private)
{
	REFLECT_GENERATED_BODY()

public:
	Person() { }
	 

	REFLECT_PROPERTY()
		int TestFuncInt() { std::cout << "TestFunc is being called"; return 12; }
	REFLECT_PROPERTY()
		const char* TestFuncChar() { std::cout << "TestFunc is being called"; return "TestFunct"; }
	
	REFLECT_PROPERTY()
	int y;
	REFLECT_PROPERTY(Friend, private)
	int x;

	REFLECT_PROPERTY()
	std::string TestString;
};

REFLECT_CLASS(House, Public, Private)
{
	REFLECT_GENERATED_BODY()

	REFLECT_PROPERTY(Friend, )
		int width;
};