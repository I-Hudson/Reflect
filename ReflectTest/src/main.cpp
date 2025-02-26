#include "Reflect/Reflect.h"
#include "TestStrcuts.h"
#include "TestClass.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include <iostream>

#ifndef REFLECT_TYPE_INFO_ENABLED
TEST_CASE("Legacy function no return")
{
	// Get a function with no return value.

	Player player;
	auto playerGetId = player.GetFunction("PrintHelloWorld");
	CHECK(playerGetId.Invoke() == Reflect::EReflectReturnCode::SUCCESS);

	auto playerGetName = player.GetFunction("GetName");
	std::string name;
	CHECK(playerGetName.Invoke(&name) == Reflect::EReflectReturnCode::SUCCESS);
	CHECK(name == player.GetName());
}

TEST_CASE("Legacy function with return value")
{
	// Get a function with a return value std::string.
	// The return value with be set to playerId.
	Player player;
	Reflect::ReflectFunction playerGetId = player.GetFunction("GetId");
	std::string playerId;
	CHECK(playerGetId.Invoke(&playerId) == Reflect::EReflectReturnCode::SUCCESS);
	CHECK(!playerId.empty());
}

TEST_CASE("Legacy function with parameters")
{
	// Get a function with a return value but which has a single parameter.
	Player player;
	Reflect::ReflectFunction parameterFunc = player.GetFunction("GetOnlineFriendsCount");

	// Setup the parameter to send to the function. This is order
	// sensitive.
	Reflect::FunctionPtrArgs args;
	int intParameter = 8;
	args.AddArg(&intParameter);

	int returnCount;
	CHECK(parameterFunc.Invoke(&returnCount, args) == Reflect::EReflectReturnCode::SUCCESS);
	CHECK(returnCount == intParameter);
}

TEST_CASE("Legacy get member with flags")
{
	Player player;
	auto member = player.GetMember("Age");
	int* playerAgePtr = member.ConvertToType<int>();
	int playerAgeValue = *playerAgePtr;

	CHECK(member.IsValid());
	CHECK(playerAgePtr);
	CHECK(playerAgeValue == player.Age);

	auto membersWithPublic = player.GetMembers({ "Public" });
	CHECK(!membersWithPublic.empty());
}

TEST_CASE("Legacy get all members")
{
	return;
	int testI = 45;
	A::B::S s;
	s.Friends = 78;
	A::B::S otherS;

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

TEST_CASE("Legacy get function pointer return")
{
	Player player;
	Reflect::ReflectFunction func = player.GetFunction("GetIdPtr");
	CHECK(func.IsValid());

	std::string* stringPtr = nullptr;
	CHECK(func.Invoke(&stringPtr) == Reflect::EReflectReturnCode::SUCCESS);
	CHECK(stringPtr != nullptr);

	*stringPtr = "Pointer ID";
	CHECK(player.Id == *stringPtr);

	std::string& stringRef = *stringPtr;
	stringRef = "ReferenceID";
	CHECK(player.Id == stringRef);
}

TEST_CASE("Legacy property inheritance")
{
	ClassHolder classHolder;
}
#endif

#ifdef REFLECT_TYPE_INFO_ENABLED
TEST_CASE("TypeInfo registry")
{
	ClassHolder classHolder;
	Reflect::TypeInfoRegistry& registy = Reflect::TypeInfoRegistry::Instance();

	Player p;
	Reflect::TypeInfo playerTypeInfo = p.GetTypeInfo();
	CHECK(playerTypeInfo.GetType().IsValid());

	Reflect::Type playerType = Reflect::Type::MakeType<Player>();
	CHECK(playerType.IsValid());
	CHECK(playerType == playerTypeInfo.GetType());
	CHECK(playerType.GetTypeId() == playerTypeInfo.GetType().GetTypeId());

	CHECK(registy.HasTypeInfo(playerType.GetTypeId()));

	Reflect::TypeInfo rPTypeInfo = registy.GetTypeInfo<Player>();
	CHECK(!rPTypeInfo.IsValid());
	CHECK(!rPTypeInfo.HasObjectInstance());
	CHECK(rPTypeInfo.GetType() == playerTypeInfo);
	CHECK(rPTypeInfo.GetType().GetTypeId() == playerTypeInfo.GetTypeId());
}

TEST_CASE("Get type info no owner")
{
	Player player;
	Reflect::TypeInfo typeinfo = Player::GetStaticTypeInfo();
	CHECK(!typeinfo.IsValid());
	CHECK(!typeinfo);
	CHECK(!typeinfo.HasObjectInstance());

	CHECK(typeinfo.GetType().IsValid());
	CHECK(!typeinfo.GetType().GetPrettyTypeName().empty());
}

TEST_CASE("Get type info with owner")
{
	Player player;
	Reflect::TypeInfo typeinfo = player.GetTypeInfo();
	CHECK(typeinfo.IsValid());
	CHECK(typeinfo);
	CHECK(typeinfo.HasObjectInstance());
}

TEST_CASE("Type info construct")
{
	//Player* constructed = static_cast<Player*>(Player::GetStaticTypeInfo().ConstructNew());
	//CHECK(constructed != nullptr);
	//delete constructed;
}

TEST_CASE("Function info invoke, return invalid_owner_object")
{
	Reflect::TypeInfo typeinfo = Player::GetStaticTypeInfo();
	Reflect::FunctionInfo getOnlineFriendsCount = typeinfo.GetFunctionInfo("GetOnlineFriendsCount");
	CHECK(getOnlineFriendsCount.Invoke() == Reflect::EReflectReturnCode::INVALID_OWNER_OBJECT);
}

TEST_CASE("Function info invoke, return invalid_function_pointer")
{
#if 0
	Player player;
	Reflect::TypeInfo typeinfo = player.GetTypeInfo();

	Reflect::FunctionInfo getOnlineFriendsCount = typeinfo.GetFunctionInfo("FAKE_FUCNTION");
	CHECK(getOnlineFriendsCount.Invoke() == Reflect::EReflectReturnCode::INVALID_FUNCTION_POINTER);
#endif
}

TEST_CASE("Member info invoke, return invalid_member")
{
#if 0
#endif
}

TEST_CASE("Function info invoke, return invalid_function_pointer")
{
	Player player;
	Reflect::TypeInfo typeinfo = player.GetTypeInfo();

	Reflect::FunctionInfo getOnlineFriendsCount = typeinfo.GetFunctionInfo("GetOnlineFriendsCount");
	CHECK(getOnlineFriendsCount.Invoke() == Reflect::EReflectReturnCode::FUNCTION_INVALID_ARGS);
}

TEST_CASE("Function info invoke, return function_invalid_return_type")
{
	Player player;
	Reflect::TypeInfo typeinfo = player.GetTypeInfo();

	Reflect::FunctionInfoArgs funcArgs;
	int maxCount = 10;
	funcArgs.AddArg(&maxCount);

	Reflect::FunctionInfo getOnlineFriendsCount = typeinfo.GetFunctionInfo("GetOnlineFriendsCount");
	bool returnValue = false;
	CHECK(getOnlineFriendsCount.Invoke(&returnValue, funcArgs) == Reflect::EReflectReturnCode::FUNCTION_INVALID_RETURN_TYPE);
}

TEST_CASE("Member info IsValid")
{
	Player player;
	Reflect::TypeInfo typeinfo = player.GetTypeInfo();

	Reflect::MemberInfo playerId = typeinfo.GetMemberInfo("Id");
	CHECK(!playerId.IsValid());
}

TEST_CASE("Member info get member pointer")
{
	Player player;
	Reflect::TypeInfo typeinfo = player.GetTypeInfo();

	Reflect::MemberInfo playerId = typeinfo.GetMemberInfo("IdReflected");
	CHECK(playerId.GetMemberPointer() != nullptr);
	std::string* idPtr = playerId.GetMemberPointer<std::string>();
	CHECK(idPtr != nullptr);
	CHECK(*idPtr == "IdReflected");
}

TEST_CASE("Type info call func with 1 arg and return")
{
	Player player;
	Reflect::TypeInfo typeinfo = player.GetTypeInfo();
	Reflect::FunctionInfo getOnlineFriendsCountFunc = typeinfo.GetFunctionInfo("GetOnlineFriendsCount");

	int onlineCount = 45;

	Reflect::FunctionInfoArgs funcArgs;
	funcArgs.AddArg(&onlineCount);

	int resultInt;
	CHECK(getOnlineFriendsCountFunc.IsValid());
	CHECK(getOnlineFriendsCountFunc.Invoke(&resultInt, funcArgs) == Reflect::EReflectReturnCode::SUCCESS);
}

TEST_CASE("Type info get base class")
{
	Player player;
	Reflect::TypeInfo typeinfo = player.GetTypeInfo();
	ClassE classE;
	Reflect::TypeInfo classETypeinfo = classE.GetTypeInfo();
	ClassHolder classHolder;
	Reflect::TypeInfo classHolderTypeinfo = classHolder.GetTypeInfo();

	Reflect::MemberInfo classEMemberInfo = classHolderTypeinfo.GetMemberInfo("ClassEObject");
	Reflect::TypeInfo classETypeInfo = Reflect::TypeInfoRegistry::GetTypeInfo(classEMemberInfo.GetTypeId(), &classHolder);
	CHECK(classETypeInfo);
	CHECK(classETypeInfo.IsValid());
	CHECK(classETypeInfo.IsDerivedFrom<ClassToRefectA>());
	//typeinfo.GetInfo().in
}

TEST_CASE("Get type info")
{
	/*Reflect::ReflectTypeMember* member = typeinfo.GetMember("TimeOnline");
	std::cout << "Member type: " << member->GetType()->GetTypeName() << '\n';

	int* memberPtr = member->ConvertToType<int>();
	std::cout << "Member value: " << *memberPtr << '\n';
	*memberPtr = 23;
	std::cout << "Member new value: " << *memberPtr << '\n';

	Reflect::ReflectTypeFunction* setAgeAndHeightFunction = typeinfo.GetFunction("SetAgeAndHeight");
	funcArgs.Clear();

	int newAge = 78;
	funcArgs.AddArg(&newAge);
	float newHeight = 5.6f;
	funcArgs.AddArg(&newHeight);

	setAgeAndHeightFunction->Invoke(funcArgs);

	auto members = typeinfo.GetAllMembersWithFlags({ "Public" });
	auto heightMember = typeinfo.GetMember("Height");
	heightMember->ModifyValue(7.5f);*/
}
#endif