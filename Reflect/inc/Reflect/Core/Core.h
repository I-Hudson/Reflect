#pragma once

#include "Log.h"
#include "Reflect/Core/Defines.h"


namespace Reflect
{
	#define REFLECT_MAJOR 5
	#define REFLECT_MINOR 0
	#define REFLECT_PATCH 2

	namespace Keys
	{
		constexpr const char* RefectStructKey = "REFLECT_STRUCT";
		constexpr const char* RefectClassKey = "REFLECT_CLASS";
		constexpr const char* ReflectContainerKeys[] = { RefectStructKey, RefectClassKey };

		constexpr const char* ReflectGeneratedBodykey = "REFLECT_GENERATED_BODY";
		constexpr const char* ReflectPropertyKey = "REFLECT_PROPERTY";
		constexpr const char* ReflectCPPIncludeKey = "REFLECT_CPP_INCLUDE";
		constexpr const char* ReflectFileGeneratePrefix = "_reflect_generated";
		constexpr const char* ReflectIgnoreStringsFileName = "reflect_ignore_strings.txt";

		constexpr const char* Reflect_CMD_Option_PCH_Include = "pchInclude";
		constexpr const char* Reflect_CMD_Option_Reflect_Full_EXT = "reflect_full_ext";
		//constexpr const char* Reflect_CMD_Option_Single_File_EXT = "reflect_single_file_ext";
	}
}

#define REFLECT_GET_VERSION() \
	 ((((uint32_t)(Reflect::REFLECT_MAJOR)) << 22) | (((uint32_t)(Reflect::REFLECT_MINOR)) << 12) | ((uint32_t)(Reflect::REFLECT_PATCH)))

#define REFLECT_VERSION_MAJOR() ((uint32_t)(REFLECT_GET_VERSION()) >> 22)
#define REFLECT_VERSION_MINOR() (((uint32_t)(REFLECT_GET_VERSION()) >> 12) & 0x3ff)
#define REFLECT_VERSION_PATCH() ((uint32_t)(REFLECT_GET_VERSION()) & 0xfff)

// Compiler marcos
#if defined(__GNUC__) || defined(__clang__)
#define REFLECT_DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201402L ) || __cplusplus >= 201402L)
#define REFLECT_DEPRECATED(x) [[deprecated(x)]]
#else
#define REFLECT_DEPRECATED(x) __declspec(deprecated(x))
#endif
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define REFLECT_DEPRECATED(func) func
#endif