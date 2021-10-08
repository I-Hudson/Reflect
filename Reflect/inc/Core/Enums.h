#pragma once

#include "Compiler.h"
#include "Core.h"
#include <string>

namespace Reflect 
{
	enum EReflectFlags
	{
		Invalid = 1 << 0,

		Private = 1 << 1,
		Public = 1 << 2,
		Friend = 1 << 3,
	};
	REFLECT_DLL EReflectFlags StringToReflectFlags(const std::string& str);

	enum class EReflectType
	{
		Class,
		Struct,
		Member,
		Function,

		Unknown
	};

	enum class EReflectValueType
	{
		Value,
		Reference,
		Pointer,
		PointerReference,
	};

	enum class EReflectMemberModifier
	{
		None,
		Const, 
		Static,
		Volatile,
		Virtual,
	};

	enum class EReflectReturnCode
	{
		SUCCESS,
		FAILED,

		CAST_FAILED,
		INVALID_FUNCTION_POINTER,
		INVALID_MEMBER,
	};
	REFLECT_DLL std::string ReflectReturnCodeToString(const EReflectReturnCode& code);
}

/// <summary>
/// Dirty way of getting the enum string value.
/// </summary>
/// <typeparam name="E"></typeparam>
/// <returns></returns>
template <typename E, E> REFLECT_DLL std::string EnumToString()
{
	std::string value = FUNC_SIG;
	int startIndex = static_cast<int>(value.find_last_of(',')) + 1;
	int endIndex = static_cast<int>(value.find_last_of('>'));
	value = value.substr(startIndex, endIndex - startIndex);
	return value;
}