#pragma once

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
#define CPP_17 1
#else
#define CPP_17 0
#endif

#if CPP_17  == 0
#pragma error C++17 must be used.
#endif

#ifdef REFLECT_DLL_EXPORT
#define REFLECT_API __declspec(dllexport)
#elif defined (REFLECT_DLL_IMPORT)
#define REFLECT_API __declspec(dllimport)
#else 
#define REFLECT_API
#endif

#define REFLECT_PROPERTY(...)
#define NO_REFLECT(x) x

#define BODY_MACRO_COMBINE_INNER(A, B, C, D) A##B##C##D
#define BODY_MACRO_COMBINE(A, B, C, D) BODY_MACRO_COMBINE_INNER(A, B, C, D)

#ifdef REFLECT_SINGLE_FILE
#define REFLECT_GENERATED_BODY(...) BODY_MACRO_COMBINE(__VA_ARGS__, _Source_h_, __LINE__, _GENERATED_BODY);
#else
#define REFLECT_GENERATED_BODY() BODY_MACRO_COMBINE(CURRENT_FILE_ID, _, __LINE__, _GENERATED_BODY);
#endif

#define REFLECT_CPP_INCLUDE(...)

#define REFLECT_STRUCT(...)
#define REFLECT_CLASS(...)


// Only use a REFLECT_CLASS/REFLECT_STRUCT for the parser and do not generate
// any additional reflected files for this object. Using this can improved the overall
// reflected code.
#define REFLECT_LOOKUP_ONLY "REFLECT_LOOKUP_ONLY"