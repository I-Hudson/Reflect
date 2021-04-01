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

enum class ReflectReturnCode
{
	SUCCESS,
	FAILED,

	CAST_FAILED,
	INVALID_FUNCTION_POINTER,
	INVALID_MEMBER,
};
std::string ReflectReturnCodeToString(const ReflectReturnCode& code);

using FunctionPtr = ReflectReturnCode(*)(void* objectPtr, void* returnValue, FunctionPtrArgs& args);

struct ReflectFunction
{
	ReflectFunction(void* objectPtr, FunctionPtr func)
		: m_objectPtr(objectPtr)
		, m_func(func)
	{ }

	//template<typename... Args>
	//void Invoke(void* returnValue, Args... args)
	//{
	//	FunctionPtrArgs funcArgs = PackFunctionArgs(std::forward<Args>(args)...);
	//	int i = *static_cast<int*>(funcArgs.GetArg(0));
	//	int* ip = static_cast<int*>(funcArgs.GetArg(1));
	//	return (*Func)(ObjectPtr, returnValue, funcArgs);
	//}

	ReflectReturnCode Invoke(void* returnValue = nullptr, FunctionPtrArgs functionArgs = FunctionPtrArgs())
	{
		if (IsValid())
		{
			(*m_func)(m_objectPtr, returnValue, functionArgs);
			return ReflectReturnCode::SUCCESS;
		}
		return ReflectReturnCode::INVALID_FUNCTION_POINTER;
	}

	bool IsValid() const
	{
		return m_objectPtr != nullptr;
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
	void* m_objectPtr;
	FunctionPtr m_func;
};

struct ReflectMember
{
	ReflectMember(const char* memberName, std::string memberType, int memberOffset)
		: m_name(memberName)
		, m_type(memberType)
		, m_offset(memberOffset)
	{}

	ReflectMember(const char* memberName, std::string memberType, void* memberPtr)
		: m_name(memberName)
		, m_type(memberType)
		, m_ptr(memberPtr)
	{}

	bool IsValid() const
	{
		return m_ptr != nullptr;
	}

	template<typename T>
	T* ConvertToType()
	{
		const char* convertType = Reflect::Util::GetTypeName<T>();
		if (convertType != m_type)
		{
			return nullptr;
		}
		return static_cast<T*>(m_ptr);
	}

private:
	const char* m_name;
	std::string m_type;
	void* m_ptr;
	int m_offset;
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