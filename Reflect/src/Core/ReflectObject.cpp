#include "Core/ReflectObject.h"

std::string ReflectReturnCodeToString(const ReflectReturnCode& code)
{
switch (code)
{
	case ReflectReturnCode::SUCCESS: return "ReflectFuncReturnCode - Success";
	case ReflectReturnCode::FAILED: return "ReflectFuncReturnCode - Failed";

	case ReflectReturnCode::CAST_FAILED: return "ReflectFuncReturnCode - Cast Failed";
	case ReflectReturnCode::INVALID_FUNCTION_POINTER: return "ReflectFuncReturnCode - Invalid Function Pointer";
	case ReflectReturnCode::INVALID_MEMBER: return "ReflectFuncReturnCode - Invalid Member";

	default: throw std::exception("[ReflectFuncReturnCodeToString] Missing ReflectFuncReturnCode to string conversion.");
}
}