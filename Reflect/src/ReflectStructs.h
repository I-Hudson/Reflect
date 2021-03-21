#pragma once

#include "Core/Core.h"
#include "Core/Enums.h"
#include <vector>
#include <functional>
#include <type_traits>
#include <unordered_map>

NAMESPACE_START

template<typename Func>
struct FuncWrapper
{
	FuncWrapper(Func func)
		: m_func(std::move(func))
	{ }

	template<typename... Args, 
		typename = std::enable_if_t<std::is_void_v<std::invoke_result<Func, Args...>>>>
	void Call(Args... args)
	{
		m_func(std::forward<Args>(args)...);
	}

	template<typename... Args, 
		typename = std::enable_if_t<!std::is_void_v<std::invoke_result<Func, Args...>>>>
	auto Call(Args... args)
	{
		return m_func(std::forward<Args>(args)...);
	}

	Func m_func;
};

template<typename Func, typename... Args>
struct InvokeFunction
{
	InvokeFunction(FuncWrapper<Func>& FuncWrapper, Args... args)
		: m_funcWrapper(FuncWrapper)
		, m_args(std::move(args))
	{ }

	template<typename = std::enable_if_t<!std::is_void_v<std::invoke_result<Func, Args...>>>>
	auto Call()
	{
		return m_funcWrapper.Call(m_args...);
	}

	FuncWrapper<Func>& m_funcWrapper;
	std::tuple<Args...> m_args;
};

struct ReflectTypeNameData
{
	std::string Type;
	std::string Name;
	int TypeSize;
	std::vector<ReflectFlags> ContainerProps;
};

struct ReflectMemberData : public ReflectTypeNameData
{
	ReflectType ReflectType = ReflectType::Member;
};

struct ReflectFunctionData : public ReflectTypeNameData
{
	ReflectType ReflectType = ReflectType::Function;
	std::vector<ReflectTypeNameData> Parameters;
};

struct ReflectContainerData : public ReflectTypeNameData
{
	std::string Name;
	ReflectType ReflectType;
	int ReflectGenerateBodyLine;

	std::vector<ReflectMemberData> Members;
	std::vector<ReflectFunctionData> Functions;
};

struct FileParsedData
{
	std::string Data;
	int Cursor;
	std::string FilePath;
	std::string FileName;
	int GeneratedBodyLineOffset;

	std::vector<ReflectContainerData> ReflectData;
};

NAMESPACE_END