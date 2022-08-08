#include "Reflect.h"
#include "TestStrcuts.h"
#include <iostream>

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
	std::cout << returnCount << '\n';
	std::cout << Reflect::ReflectReturnCodeToString(parameterFunc.Invoke(&returnCount, args)) << '\n';
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
	return;
	int testI = 45;
	S s;
	s.Friends = 78;
	S otherS;

	auto allMembers = s.GetAllMembers();
	auto member = allMembers[0];
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

	Log_Info("Typename: %s, Typesize: %i", member.GetType()->GetTypeName().c_str(), member.GetType()->GetTypeSize());
	member.GetType()->Copy(member.GetRawPointer(), otherS.GetMember("Friends").GetRawPointer());
	member.GetType()->ClearValue(valuePtr);
	member.GetType()->Copy(valuePtr, copyPtr);

	std::cout << "S member count: " << allMembers.size() << '\n';
}

void GetFunctionRefReturn()
{
	Player player;
	Reflect::ReflectFunction func = player.GetFunction("GetIdPtr");
	if (!func.IsValid())
	{
		return;
	}

	std::string* baseStringPtr;
	func.Invoke(&baseStringPtr);
	std::string* stringPtr = baseStringPtr;
	std::string& stringRef = *baseStringPtr;
	*stringPtr = "Pointer ID";
	stringRef = "ReferenceID";
}

void GetTypeInfo()
{
#ifdef REFLET_TYPE_INFO
	Player player;
	Reflect::ReflectTypeInfo typeinfo = Player::GetTypeInfo();
	Reflect::ReflectTypeFunction* getOnlineFriendsCount = typeinfo.GetFunction("GetOnlineFriendsCount");

	std::cout << "getOnlineFriendsCount valid: " << getOnlineFriendsCount->IsValid() << '\n';
	std::cout << "getOnlineFriendsCount invoke: " << ReflectReturnCodeToString(getOnlineFriendsCount->Invoke()) << '\n';

	typeinfo = Player::GetTypeInfo(&player);
	getOnlineFriendsCount = typeinfo.GetFunction("GetOnlineFriendsCount");

	int onlineCount = 45;

	Reflect::FunctionPtrArgs funcArgs;
	funcArgs.AddArg(&onlineCount);

	int resultInt;

	std::cout << "getOnlineFriendsCount valid: " << getOnlineFriendsCount->IsValid() << '\n';
	std::cout << "getOnlineFriendsCount invoke: " << ReflectReturnCodeToString(getOnlineFriendsCount->Invoke(&resultInt, funcArgs)) << '\n';
	std::cout << "getOnlineFriendsCount return: " << resultInt << '\n';
#endif
}

int main(void)
{
	FuncNoReturn();
	FuncReturnValue();
	FuncWithParameters();
	GetMemberWithFlags();
	GetAllMemebers();
	GetTypeInfo();
	GetFunctionRefReturn();

	return 0;
}