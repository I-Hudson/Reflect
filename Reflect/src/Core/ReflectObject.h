#pragma once

#include "ReflectStructs.h"
#include "Core/Util.h"
#include <assert.h>
#include <utility>


/// <summary>
/// Store arguments for a function ptr
/// </summary>
struct FunctionPtrArgs
{
public:
	struct Arg
	{
		Arg(std::string type, void* ptr)
			: Type(type)
			, Ptr(ptr)
		{ }

		void* Get() const { return Ptr; }

	private:
		std::string Type;
		void* Ptr;
	};

	FunctionPtrArgs() { }
	FunctionPtrArgs(const std::vector<Arg>& args)
		: m_args(args)
	{ }

	void* GetArg(int index)
	{
		return m_args.at(index).Get();
	}

	//template<typename T>
	//void AddArg(const T& obj)
	//{
	//	m_args.push_back(Arg(Reflect::Util::GetTypeName(obj), &obj));
	//}

	template<typename T>
	void AddArg(T* obj)
	{
		m_args.push_back(Arg(Reflect::Util::GetTypeName(*obj), obj));
	}

private:
	std::vector<Arg> m_args;
};

using FunctionPtr = void (*)(void* objectPtr, void* returnValue, FunctionPtrArgs& args);

struct ReflectFunction
{
	ReflectFunction(void* objectPtr, FunctionPtr func)
		: ObjectPtr(objectPtr)
		, Func(func)
	{ }

	//template<typename... Args>
	//void Invoke(void* returnValue, Args... args)
	//{
	//	FunctionPtrArgs funcArgs = PackFunctionArgs(std::forward<Args>(args)...);
	//	int i = *static_cast<int*>(funcArgs.GetArg(0));
	//	int* ip = static_cast<int*>(funcArgs.GetArg(1));
	//	return (*Func)(ObjectPtr, returnValue, funcArgs);
	//}

	void Invoke(void* returnValue, FunctionPtrArgs functionArgs)
	{
		return (*Func)(ObjectPtr, returnValue, functionArgs);
	}

private:
	template<typename... Args>
	FunctionPtrArgs PackFunctionArgs(Args... args)
	{
		std::vector<FunctionPtrArgs::Arg> funcArgs = { PackFunctionArg(args)... };
		return  FunctionPtrArgs(funcArgs);
	}

	template<typename T, typename... Args>
	FunctionPtrArgs::Arg PackFunctionArg(T& t, Args&... args)
	{
		return FunctionPtrArgs::Arg(Reflect::Util::GetTypeName(t), &t);
	}

	template<typename T, typename... Args>
	FunctionPtrArgs::Arg PackFunctionArg(T* t, Args... args)
	{
		return FunctionPtrArgs::Arg(Reflect::Util::GetTypeName(t), static_cast<void*>(t));
	}

private:
	void* ObjectPtr;
	FunctionPtr Func;
};

struct ReflectMember
{
	ReflectMember(const char* memberName, std::string memberType, int memberOffset)
		: Name(memberName)
		, Type(memberType)
		, Offset(memberOffset)
	{}

	ReflectMember(const char* memberName, std::string memberType, void* memberPtr)
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
	std::string Type;
	void* Ptr;
	int Offset;
};

class ReflectObject
{
public:
	ReflectObject() { }
	~ReflectObject() { }

	virtual const char* Verify() = 0;

protected:
	//template<typename Func>
	//void AddFunction(const char* functionName, Reflect::FuncWrapper<Func> funcWrapper) { }
	//void AddMember(const char* memberName, void* memberPtr);
};