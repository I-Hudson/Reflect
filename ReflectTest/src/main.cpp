#include "Reflect.h"
#include "TestStrcuts.h"
#include <iostream>

void FuncNoReturn()
{
	// Get a function with no return value.
	Player player;
	auto playerGetId = player.GetFunction("PrintHelloWorld");
	playerGetId.Invoke();
}

void FuncReturnValue()
{
	// Get a function with a return value std::string.
	// The return value with be set to playerId.
	Player player;
	Reflect::ReflectFunction playerGetId = player.GetFunction("GetId");
	std::string playerId;
	playerGetId.Invoke(&playerId);
}

void FuncWithParameters()
{
	// Get a function with no return value but which has a single
	// parameter.
	Player player;
	Reflect::ReflectFunction parameterFunc = player.GetFunction("GetOnlineFriendsCount");
	
	// Setup the parameter to send to the function. This is order
	// sensitive.
	Reflect::FunctionPtrArgs args;
	int intParameter = 8;
	args.AddArg(&intParameter);

	int returnCount;
	std::cout << Reflect::ReflectReturnCodeToString(parameterFunc.Invoke(&returnCount, args));
	std::cout << Reflect::ReflectReturnCodeToString(parameterFunc.Invoke(&returnCount, args));
}

int main(void)
{
	FuncNoReturn();
	FuncReturnValue();
	FuncWithParameters();

	return 0;
}