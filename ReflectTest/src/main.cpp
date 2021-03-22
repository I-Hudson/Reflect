#include "Reflect.h"
#include "TestStrcuts.h"
#include <iostream>

int main(void)
{
	std::cout << "Compiled" << std::endl;

	Person person;
	auto value = person.GetMember("TestString");
	
	auto* str = value.ConvertToType<std::string>();
	//*str = "This is a test string set in person.";

	return 0;
}