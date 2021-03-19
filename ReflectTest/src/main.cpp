#include "Reflect.h"
#include "TestStrcuts.h"
#include <iostream>

int main(void)
{
	std::cout << "Compiled" << std::endl;

	Person p;
	std::cout << p.Verify() << std::endl;
	auto func = p.GetFunction("");

	return 0;
}