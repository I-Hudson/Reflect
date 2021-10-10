#pragma once

#include "Log.h"

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
#define CPP_17 1
#else
#define CPP_17 0
#endif

#if CPP_17  == 0
#pragma error C++17 must be used.
#endif

#ifdef REFLECT_DLL_EXPORT
#define REFLECT_DLL __declspec(dllexport)
#elif defined (REFLECT_DLL_IMPORT)
#define REFLECT_DLL __declspec(dllimport)
#else 
#define REFLECT_DLL
#endif

#define EXP_PARSER

#define REFLECT_PROPERTY(...)
#define NO_REFLECT(x) x

#define BODY_MACRO_COMBINE_INNER(A, B, C, D) A##B##C##D
#define BODY_MACRO_COMBINE(A, B, C, D) BODY_MACRO_COMBINE_INNER(A, B, C, D)

#define REFLECT_GENERATED_BODY(...) BODY_MACRO_COMBINE(CURRENT_FILE_ID, _, __LINE__, _GENERATED_BODY);

namespace Reflect
{
	#define REFLECT_MAJOR 3
	#define REFLECT_MINOR 1
	#define REFLECT_PATCH 1

	constexpr const char* RefectStructKey = "REFLECT_STRUCT";
	constexpr const char* RefectClassKey = "REFLECT_CLASS";
	constexpr const char* ReflectGeneratedBodykey = "REFLECT_GENERATED_BODY";
	constexpr const char* ReflectPropertyKey = "REFLECT_PROPERTY";
	constexpr const char* ReflectFileGeneratePrefix = "_reflect_generated";
	constexpr const char* ReflectIgnoreStringsFileName = "reflect_ignore_strings.txt";
}

#define REFLECT_GET_VERSION() \
	 ((((uint32_t)(Reflect::REFLECT_MAJOR)) << 22) | (((uint32_t)(Reflect::REFLECT_MINOR)) << 12) | ((uint32_t)(Reflect::REFLECT_PATCH)))

#define REFLECT_VERSION_MAJOR() ((uint32_t)(REFLECT_GET_VERSION()) >> 22)
#define REFLECT_VERSION_MINOR() (((uint32_t)(REFLECT_GET_VERSION()) >> 12) & 0x3ff)
#define REFLECT_VERSION_PATCH() ((uint32_t)(REFLECT_GET_VERSION()) & 0xfff)

// Compiler marcos
#if defined(__GNUC__) || defined(__clang__)
#define DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201402L ) || __cplusplus >= 201402L)
#define DEPRECATED(x) [[deprecated(x)]]
#else
#define DEPRECATED(x) __declspec(deprecated(x))
#endif
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED(func) func
#endif