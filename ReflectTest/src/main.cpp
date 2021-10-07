#include "Reflect.h"
#include "TestStrcuts.h"
#include <iostream>

#include "Core/ReflectData.h"

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

void GetMemberWithFlags()
{
	Player player;
	auto member = player.GetMember("");
	auto membersWithPublic = player.GetMembers({ "Public" });
	int& friendInt = *membersWithPublic[0].ConvertToType<int>();
	friendInt = 12;
}

int main(void)
{
	FuncNoReturn();
	FuncReturnValue();
	FuncWithParameters();
	GetMemberWithFlags();

	int* i;
	int *& ptrRef = i;

	Reflect::EXP::ReflectTypeTemplate<std::string> m_nameType;
	std::string* strPtr = m_nameType.ConvertObjPtrToReturnValue<std::string>();
	*strPtr = "This is a type name.";
	return 0;
}