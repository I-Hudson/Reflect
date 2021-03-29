#include "Core/Enums.h"
#include "Core/Util.h"

NAMESPACE_START

ReflectFlags StringToReflectFlags(const std::string& str)
{
	if (Util::ToLower(str) == "private") return ReflectFlags::Private;
	else if (Util::ToLower(str) == "public") return ReflectFlags::Public;
	else if (Util::ToLower(str) == "friend") return ReflectFlags::Friend;
	return ReflectFlags::Invalid;
}

NAMESPACE_END