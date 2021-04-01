 // This file is auto generated please don't modify.
#include "Core/ReflectObject.h"
#include "Core/Util.h"

#ifdef TestStrcuts_reflect_generated_h
#error "TestStrcuts_reflect_generated.h already included, missing 'pragma once' in TestStrcuts.h"
#endif TestStrcuts_reflect_generated_h
#define TestStrcuts_reflect_generated_h

#define TestStrcuts_Source_h_9_PROPERTIES \
private:\
	static Reflect::ReflectMemberProp __REFLECT_MEMBER_PROPS__[3];\


#define TestStrcuts_Source_h_9_FUNCTION_DECLARE \
private:\
	static void __REFLECT_FUNC__TestFuncInt(void* objectPtr, void* returnValuePtr, FunctionPtrArgs& functionArgs = FunctionPtrArgs())\
	{\
		int const* testIntArg = static_cast<int const*>(functionArgs.GetArg(0));\
		const int& testIntPtrArg = *static_cast<const int*>(functionArgs.GetArg(1));\
		Person* ptr = static_cast<Person*>(objectPtr);\
		*((int*)returnValuePtr) = ptr->TestFuncInt(testIntArg, testIntPtrArg);\
	}\
	static void __REFLECT_FUNC__TestFuncChar(void* objectPtr, void* returnValuePtr, FunctionPtrArgs& functionArgs = FunctionPtrArgs())\
	{\
		Person* ptr = static_cast<Person*>(objectPtr);\
		*((const char**)returnValuePtr) = ptr->TestFuncChar();\
	}\
	static void __REFLECT_FUNC__TestFuncVoid(void* objectPtr, void* returnValuePtr, FunctionPtrArgs& functionArgs = FunctionPtrArgs())\
	{\
		Person* ptr = static_cast<Person*>(objectPtr);\
		ptr->TestFuncVoid();\
	}\


#define TestStrcuts_Source_h_9_FUNCTION_GET \
public:\
	ReflectFunction GetFunction(const char* functionName);\


#define TestStrcuts_Source_h_9_PROPERTIES_OFFSET \
private:\
	static int __REFLECT__y() { return offsetof(Person, y); }; \
	static int __REFLECT__x() { return offsetof(Person, x); }; \
	static int __REFLECT__TestString() { return offsetof(Person, TestString); }; \


#define TestStrcuts_Source_h_9_PROPERTIES_GET \
public:\
ReflectMember GetMember(const char* memberName);\
std::vector<ReflectMember> GetMembers(unsigned int flags);\


#define TestStrcuts_Source_h_9_GENERATED_BODY \
TestStrcuts_Source_h_9_PROPERTIES \
TestStrcuts_Source_h_9_FUNCTION_DECLARE \
TestStrcuts_Source_h_9_FUNCTION_GET \
TestStrcuts_Source_h_9_PROPERTIES_OFFSET \
TestStrcuts_Source_h_9_PROPERTIES_GET \


#define TestStrcuts_Source_h_32_PROPERTIES \
private:\
	static Reflect::ReflectMemberProp __REFLECT_MEMBER_PROPS__[2];\


#define TestStrcuts_Source_h_32_FUNCTION_DECLARE \
private:\


#define TestStrcuts_Source_h_32_FUNCTION_GET \
public:\
	ReflectFunction GetFunction(const char* functionName);\


#define TestStrcuts_Source_h_32_PROPERTIES_OFFSET \
private:\
	static int __REFLECT__width() { return offsetof(House, width); }; \
	static int __REFLECT__m_person() { return offsetof(House, m_person); }; \


#define TestStrcuts_Source_h_32_PROPERTIES_GET \
public:\
ReflectMember GetMember(const char* memberName);\
std::vector<ReflectMember> GetMembers(unsigned int flags);\


#define TestStrcuts_Source_h_32_GENERATED_BODY \
TestStrcuts_Source_h_32_PROPERTIES \
TestStrcuts_Source_h_32_FUNCTION_DECLARE \
TestStrcuts_Source_h_32_FUNCTION_GET \
TestStrcuts_Source_h_32_PROPERTIES_OFFSET \
TestStrcuts_Source_h_32_PROPERTIES_GET \


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID TestStrcuts_Source_h
