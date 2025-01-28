#pragma once

#include "Reflect/Core/Defines.h"

namespace Reflect 
{
	enum EReflectFlags
	{
		Invalid = 1 << 0,

		Private = 1 << 1,
		Public = 1 << 2,
		Friend = 1 << 3,
	};
	REFLECT_API EReflectFlags StringToReflectFlags(const char* str);

	enum class EReflectType
	{
		Class,
		Struct,
		Member,
		Function,
		Parameter,

		Unknown
	};

	enum class EReflectValueType
	{
		Value,
		Reference,
		Pointer,
		PointerReference,

		Unknown
	};

	enum class EReflectValueModifier
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
		INVALID_OWNER_OBJECT,			// There is not a valid owner object instance pointer
		INVALID_FUNCTION_POINTER,		// There is not a valid function pointer

		FUNCTION_INVALID_ARGS,			// The arguments are not valid for the invoked function
		FUNCTION_INVALID_RETURN_TYPE,	// The return type is not the same as the function's
	};
	REFLECT_API const char* ReflectReturnCodeToString(const EReflectReturnCode& code);
}