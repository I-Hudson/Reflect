#include "Reflect.h"
#include "TestStrcuts.h"
#include <iostream>

int main(void)
{
	std::cout << "Compiled" << std::endl;

	Person person;
	auto value = person.GetMember("y");
	int& typeInt = *value.ConvertToType<int>();
	typeInt = 54;
	auto func = person.GetFunction("TestFuncInt");

	int const&  constI = 0;
	int returnV;
	int p1 = 25;
	int p2 = 10;
	int pp1 = 25;
	FunctionPtrArgs args;
	args.AddArg(&p1);
	args.AddArg(&pp1);
	func.Invoke(&returnV, args);

	return 0;
}