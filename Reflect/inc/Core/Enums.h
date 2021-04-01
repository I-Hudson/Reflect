#pragma once

#include "Compiler.h"
#include "Core.h"
#include <string>

NAMESPACE_START

enum ReflectFlags
{
	Invalid = 1 << 0,

	Private = 1 << 1,
	Public = 1 << 2,
	Friend = 1 << 3,
};
ReflectFlags StringToReflectFlags(const std::string& str);

enum class ReflectType
{
	Class,
	Struct,
	Member,
	Function,

	Count
};

enum class ReflectMemberType
{
	Value,
	Reference,
	Pointer,

	Count
};

NAMESPACE_END

/// <summary>
/// Dirty way of getting the enum string value.
/// </summary>
/// <typeparam name="E"></typeparam>
/// <returns></returns>
template <typename E, E> constexpr std::string EnumToString()
{
	std::string value = FUNC_SIG;
	int startIndex = static_cast<int>(value.find_last_of(',')) + 1;
	int endIndex = static_cast<int>(value.find_last_of('>'));
	value = value.substr(startIndex, endIndex - startIndex);
	return value;
}