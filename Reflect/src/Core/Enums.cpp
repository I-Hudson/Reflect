#include "Core/Enums.h"
#include "Core/Util.h"

NAMESPACE_START

ReflectFlags StringToReflectFlags(const std::string& str)
{
	if (Util::ToLower(str) == "private" || EnumToString<ReflectFlags, Private>() == str) return ReflectFlags::Private;
	else if (Util::ToLower(str) == "public" || EnumToString<ReflectFlags, Public>() == str) return ReflectFlags::Public;
	else if (Util::ToLower(str) == "friend" || EnumToString<ReflectFlags, Friend>() == str) return ReflectFlags::Friend;
	return ReflectFlags::Invalid;
}

NAMESPACE_END