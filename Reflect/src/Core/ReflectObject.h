#pragma once

#include "ReflectStructs.h"
#include "Core/Util.h"
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
	ReflectMember(const char* memberName, const char* memberType, void* memberPtr)
		: Name(memberName)
		, Type(memberType)
		, Ptr(memberPtr)
	{}

	bool IsValid() const
	{
		return Ptr != nullptr;
	}

	template<typename T>
	T* ConvertToType()
	{ 
		const char* convertType = Reflect::Util::GetTypeName<T>();
		if (convertType != Type)
		{
			return nullptr;
		}
		return static_cast<T*>(Ptr);
	}

private:
	const char* Name;
	const char* Type;
	void* Ptr;
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