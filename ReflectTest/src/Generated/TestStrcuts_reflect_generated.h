 // This file is auto generated please don't modify.
#include "ReflectStructs.h"
#include "Core/Util.h"

#ifdef TestStrcuts_reflect_generated_h
#error "TestStrcuts_reflect_generated.h already included, missing 'pragma once' in TestStrcuts.h"
#endif TestStrcuts_reflect_generated_h
#define TestStrcuts_reflect_generated_h

#define TestStrcuts_Source_h_11_PROPERTIES \
private:\
	static Reflect::ReflectMemberProp __REFLECT_MEMBER_PROPS__[2];\


#define TestStrcuts_Source_h_11_FUNCTION_DECLARE \
private:\
	static Reflect::ReflectReturnCode __REFLECT_FUNC__GetOnlineFriendsCount(void* objectPtr, void* returnValuePtr, Reflect::FunctionPtrArgs& functionArgs)\
	{\
		int const& maxPlayerCountArg = *static_cast<int const*>(functionArgs.GetArg(0));\
		Player* ptr = static_cast<Player*>(objectPtr);\
		*((int*)returnValuePtr) = ptr->GetOnlineFriendsCount(maxPlayerCountArg);\
		return Reflect::ReflectReturnCode::SUCCESS;\
	}\
	static Reflect::ReflectReturnCode __REFLECT_FUNC__PrintHelloWorld(void* objectPtr, void* returnValuePtr, Reflect::FunctionPtrArgs& functionArgs)\
	{\
		Player* ptr = static_cast<Player*>(objectPtr);\
		ptr->PrintHelloWorld();\
		return Reflect::ReflectReturnCode::SUCCESS;\
	}\
	static Reflect::ReflectReturnCode __REFLECT_FUNC__GetId(void* objectPtr, void* returnValuePtr, Reflect::FunctionPtrArgs& functionArgs)\
	{\
		Player* ptr = static_cast<Player*>(objectPtr);\
		*((std::string*)returnValuePtr) = ptr->GetId();\
		return Reflect::ReflectReturnCode::SUCCESS;\
	}\


#define TestStrcuts_Source_h_11_FUNCTION_GET \
public:\
	virtual Reflect::ReflectFunction GetFunction(const char* functionName) override;\


#define TestStrcuts_Source_h_11_PROPERTIES_OFFSET \
private:\
	static int __REFLECT__Friends() { return offsetof(Player, Friends); }; \
	static int __REFLECT__TimeOnline() { return offsetof(Player, TimeOnline); }; \


#define TestStrcuts_Source_h_11_PROPERTIES_GET \
public:\
virtual Reflect::ReflectMember GetMember(const char* memberName) override;\
virtual std::vector<Reflect::ReflectMember> GetMembers(std::vector<std::string> const& flags) override;\


#define TestStrcuts_Source_h_11_GENERATED_BODY \
TestStrcuts_Source_h_11_PROPERTIES \
TestStrcuts_Source_h_11_FUNCTION_DECLARE \
TestStrcuts_Source_h_11_FUNCTION_GET \
TestStrcuts_Source_h_11_PROPERTIES_OFFSET \
TestStrcuts_Source_h_11_PROPERTIES_GET \


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID TestStrcuts_Source_h
