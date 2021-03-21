#pragma once

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
#define CPP_17 1
#else
#define CPP_17 0
#endif

#if CPP_17  == 0
#pragma error C++17 must be used.
#endif

#define NAMESPACE_START namespace Reflect {
#define NAMESPACE_END }

#define REFLECT_PROPERTY(...)

#define BODY_MACRO_COMBINE_INNER(A, B, C, D) A##B##C##D
#define BODY_MACRO_COMBINE(A, B, C, D) BODY_MACRO_COMBINE_INNER(A, B, C, D)

#define REFLECT_GENERATED_BODY(...) BODY_MACRO_COMBINE(CURRENT_FILE_ID, _, __LINE__, _GENERATED_BODY);

#define REFLECT_HEADER(Name) Name

#define REFLECT_STRUCT(Name, ...) struct REFLECT_HEADER(Name)

#define REFLECT_CLASS(Name, ...) class REFLECT_HEADER(Name)

constexpr const char* RefectStructKey = "REFLECT_STRUCT";
constexpr const char* RefectClassKey = "REFLECT_CLASS";
constexpr const char* ReflectGeneratedBodykey = "REFLECT_GENERATED_BODY";
constexpr const char* PropertyKey = "REFLECT_PROPERTY";

constexpr const char* ReflectFileGeneratePrefix = "_reflect_generated";