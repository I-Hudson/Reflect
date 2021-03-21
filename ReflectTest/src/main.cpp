#include "Reflect.h"
#include "TestStrcuts.h"
#include <iostream>

template<class T> struct the_type { using type = T; };

int main(void)
{
	std::cout << "Compiled" << std::endl;

	Person person;
	auto value = person.GetMember("TestString");
	
	std::string* str = value.ConvertToType<std::string*>();
	*str = "This is a test string set in person.";

	return 0;
}