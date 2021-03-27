#pragma once

#include "Reflect.h"
#include "Generated/TestStrcuts_reflect_generated.h"
#include <iostream>

REFLECT_CLASS(Person, Public, Private)
{
	REFLECT_GENERATED_BODY()

public:
	Person() { }
	
	REFLECT_PROPERTY(private)
		int y;
	REFLECT_PROPERTY(private)
		int x;

	REFLECT_PROPERTY()
		int TestFuncInt(int const* testInt, const int& testIntPtr) { std::cout << "TestFunc is being called"; return *testInt + testIntPtr; }
	REFLECT_PROPERTY()
		const char* TestFuncChar() { std::cout << "TestFuncChar"; return ""; }
	REFLECT_PROPERTY()
		void TestFuncVoid() { std::cout << "TestFunc is being called"; }

	REFLECT_PROPERTY()
	std::string TestString;
};

REFLECT_CLASS(House, Public, Private)
{
	REFLECT_GENERATED_BODY()

	REFLECT_PROPERTY(Friend, )
		int width;

	REFLECT_PROPERTY()
	Person m_person;
};