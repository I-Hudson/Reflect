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

#define REFLECT_PROPERTY(Flags)

// TODO make sure all reflect files use this #define and include the generated file.
#define REFLECT_STRUCT(Name, Flags) struct Name : public Name##ReflectObject
#define REFLECT_CLASS(Name, Flags) class Name : public Name##ReflectObject

constexpr const char* RefectStructKey = "REFLECT_STRUCT";
constexpr const char* RefectClassKey = "REFLECT_CLASS";
constexpr const char* PropertyKey = "REFLECT_PROPERTY"; 

constexpr const char* ReflectFileGeneratePrefix = "_reflect_generated";