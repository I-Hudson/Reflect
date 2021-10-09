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

	auto playerGetName = player.GetFunction("GetName");
	std::string name;
	playerGetName.Invoke(&name);
	std::cout << name << '\n';
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
	parameterFunc.Invoke(&returnCount, args);
	std::cout << returnCount;
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

void GetAllMemebers()
{
	int testI = 45;
	S s;
	auto allMembers = s.GetAllMembers();
	auto member = allMembers[2];
	int* iptr = member.ConvertToType<int>();

	//int value = 128;
	//int copy;
	vec3 value(0);
	value.x = 12;
	value.y = 120;
	value.z = 258;
	int copy(0);

	void* valuePtr = &value;
	void* copyPtr = &copy;

	member.Type->Copy_s(valuePtr, nullptr, sizeof(decltype(copy)));
	member.Type->ClearValue(valuePtr);
	member.Type->Copy(valuePtr, copyPtr);

	std::cout << "S member count: " << allMembers.size() << '\n';
}

int main(void)
{
	//FuncNoReturn();
	//FuncReturnValue();
	//FuncWithParameters();
	//GetMemberWithFlags();
	GetAllMemebers();
	return 0;
}