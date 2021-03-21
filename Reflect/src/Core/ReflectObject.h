#pragma once

#include "ReflectStructs.h"
#include <assert.h>
#include <utility>

template<typename FuncWrapper>
struct ReflectFunction
{
	ReflectFunction(FuncWrapper funcWrapper)
		: FuncWrapper(std::move(funcWrapper))
	{ }

	const char* Name;
	FuncWrapper FuncWrapper;
};

struct ReflectMember
{
	const char* Name;
};

class ReflectObject
{
public:
	ReflectObject();
	~ReflectObject();

	virtual const char* Verify() = 0;

protected:
	template<typename Func>
	void AddFunction(const char* functionName, Reflect::FuncWrapper<Func> funcWrapper) { }
	void AddMember(const char* memberName, void* memberPtr);

protected:
	bool m_reflectInit;
	// function map
	// member variables map
};