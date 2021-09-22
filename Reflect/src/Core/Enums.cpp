#include "Core/Enums.h"
#include "Core/Util.h"
#include <stdexcept>

namespace Reflect
{
	ReflectFlags StringToReflectFlags(const std::string& str)
	{
		if (Util::ToLower(str) == "private" || EnumToString<ReflectFlags, Private>() == str) return ReflectFlags::Private;
		else if (Util::ToLower(str) == "public" || EnumToString<ReflectFlags, Public>() == str) return ReflectFlags::Public;
		else if (Util::ToLower(str) == "friend" || EnumToString<ReflectFlags, Friend>() == str) return ReflectFlags::Friend;
		return ReflectFlags::Invalid;
	}

	std::string ReflectReturnCodeToString(const ReflectReturnCode& code)
	{
		switch (code)
		{
			case ReflectReturnCode::SUCCESS: return "ReflectFuncReturnCode - Success";
			case ReflectReturnCode::FAILED: return "ReflectFuncReturnCode - Failed";

			case ReflectReturnCode::CAST_FAILED: return "ReflectFuncReturnCode - Cast Failed";
			case ReflectReturnCode::INVALID_FUNCTION_POINTER: return "ReflectFuncReturnCode - Invalid Function Pointer";
			case ReflectReturnCode::INVALID_MEMBER: return "ReflectFuncReturnCode - Invalid Member";

			default: throw std::runtime_error("[ReflectFuncReturnCodeToString] Missing ReflectFuncReturnCode to string conversion.");
		}
	}
}
