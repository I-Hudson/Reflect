#include "Reflect.h"
#include "TestStrcuts.h"
#include <iostream>

struct T
{
	int x;
};

int main(void)
{
	std::cout << "Compiled" << std::endl;

	Person p;
	std::cout << p.Verify() << std::endl;
	//auto func = p.GetReflectFunction("");
	auto iV = p.GetFunction("TestFuncInt");
	auto cV = p.GetFunction("TestFuncChar");

	T tPtr;
	int offset = offsetof(struct T, x);
	auto* vPtr = reinterpret_cast<int*>(&tPtr + offset);
	*vPtr = 25;
	auto value = *vPtr;

	return 0;
}