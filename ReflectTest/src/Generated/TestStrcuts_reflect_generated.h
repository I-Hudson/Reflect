 // This file is auto generated please don't modify.
#include "Core/ReflectObject.h"
#include <assert.h>
#include "TestStrcuts.h"

#ifdef TestStrcuts_reflect_generated_h
#error "TestStrcuts_reflect_generated.h already included, missing 'pragma once' in TestStrcuts.h"
#endif TestStrcuts_reflect_generated_h
#define TestStrcuts_reflect_generated_h

#define REFLECT_GET_FUNCTION(ObjectType, FuncName, ObjectPtr)\
if(name == "FuncName")\
{\
	auto FuncBind = std::bind(&ObjectType::FuncName, ObjectPtr);\
	Reflect::FuncWrapper FuncWrapper(FuncBind);\
	ReflectFunction FunctionPtr(FuncWrapper);\
	return FunctionPtr;\
}

class Person;
class House;

template<typename T>
class PersonReflectObject : ReflectObject
{
public:
	PersonReflectObject() { }
	~PersonReflectObject() { }
	void ReflectInit() { }
	auto GetFunction(const char* name)
	{
		if (name == "TestFuncInt") { return PersonTestFuncInt(this); }
		else if (name == "TestFuncChar") { return PersonTestFuncChar(this); }
	}
	ReflectMember GetMember(const char* name);
	virtual const char* Verify() override { return "Person"; } 


private:
	static int PersonTestFuncInt(void* ptr)
	{
		return ((T*)ptr)->TestFuncInt();
	}
	static const char* PersonTestFuncChar(void* ptr)
	{
		return ((T*)ptr)->TestFuncChar();
	}
};

template<typename T>
class HouseReflectObject : ReflectObject
{
public:
	HouseReflectObject() { }
	~HouseReflectObject() { }
	void ReflectInit() { }
	auto GetFunction(const char* name)
	{
	}
	ReflectMember GetMember(const char* name);
	virtual const char* Verify() override { return "House"; } 
protected:
};

#define TestStruct_Source_h_9_GENERATED_BODY

#undef CURRENT_FILE_ID 
#define CURRENT_FILE_ID TestStruct_Source_h