#pragma once

#include <assert.h>

struct ReflectFunction
{
	const char* Name;
};

struct ReflectMember
{
	const char* Name;
};

struct ReflectObject
{
	ReflectFunction GetFunction(const char* name);
	ReflectMember GetMember(const char* name);

	virtual const char* Verify() = 0;

protected:
	void AddFunction(const char* functionName, void* functionPtr);
	void AddMember(const char* memberName, void* memberPtr);

private:
	// function map
	// member variables map
};

#define REFLECT_STRUCT(Name, Flags) struct Name : public Name##ReflectObject
#define REFLECT_CLASS(Name, Flags) class Name : public Name##ReflectObject