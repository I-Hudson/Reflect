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