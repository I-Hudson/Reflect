#include "Enums.h"
#include "Util.h"

NAMESPACE_START

ReflectContainerProps StringToReflectContainerProps(const std::string& str)
{
	if (Util::ToLower(str) == "private") return ReflectContainerProps::Private;
	else if (Util::ToLower(str) == "public") return ReflectContainerProps::Public;
	else if (Util::ToLower(str) == "friend") return ReflectContainerProps::Friend;
	return ReflectContainerProps::Invalid;
}

NAMESPACE_END