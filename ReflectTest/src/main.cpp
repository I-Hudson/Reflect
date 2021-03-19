#include "Reflect.h"
#include "TestStrcuts.h"
#include <iostream>

int main(void)
{
	Person p;
	ReflectObject* reflect = &p;

	std::cout << "Compiled" << std::endl;
	std::cout << p.Verify() << std::endl;
	std::cout << reflect->Verify() << std::endl;

	return 0;
}