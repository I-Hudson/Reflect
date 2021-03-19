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

class  ReflectObject
{
public:
	ReflectObject();
	~ReflectObject();

	virtual ReflectFunction GetFunction(const char* name) = 0;
	virtual ReflectMember GetMember(const char* name) = 0;

	virtual const char* Verify() = 0;

protected:
	ReflectFunction GetFunctionBase(const char* name);
	ReflectMember GetMemberBase(const char* name);

	void AddFunction(const char* functionName, void* functionPtr);
	void AddMember(const char* memberName, void* memberPtr);
	virtual void SetupReflectBindings() = 0;

protected:
	bool m_reflectInit;
	// function map
	// member variables map
};