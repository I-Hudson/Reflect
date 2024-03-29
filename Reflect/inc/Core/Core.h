#pragma once

#include "Log.h"
#include <unordered_map>
#include <string_view>


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

namespace Reflect
{
	#define REFLECT_MAJOR 4
	#define REFLECT_MINOR 3
	#define REFLECT_PATCH 2

	namespace Keys
	{
		constexpr const char* RefectStructKey = "REFLECT_STRUCT";
		constexpr const char* RefectClassKey = "REFLECT_CLASS";
		constexpr const char* ReflectGeneratedBodykey = "REFLECT_GENERATED_BODY";
		constexpr const char* ReflectPropertyKey = "REFLECT_PROPERTY";
		constexpr const char* ReflectCPPIncludeKey = "REFLECT_CPP_INCLUDE";
		constexpr const char* ReflectFileGeneratePrefix = "_reflect_generated";
		constexpr const char* ReflectIgnoreStringsFileName = "reflect_ignore_strings.txt";

		constexpr const char* Reflect_CMD_Option_PCH_Include = "pchInclude";
		constexpr const char* Reflect_CMD_Option_Reflect_Full_EXT = "reflect_full_ext";
		//constexpr const char* Reflect_CMD_Option_Single_File_EXT = "reflect_single_file_ext";
	}

	constexpr const char* Reflect_CMD_Options[] =
	{
		Keys::Reflect_CMD_Option_PCH_Include
		, Keys::Reflect_CMD_Option_Reflect_Full_EXT
		//, Reflect_CMD_Option_Single_File_EXT
	};

	struct ReflectAddtionalOptions
	{
		std::string_view GetOption(std::string_view optionKey) const
		{
			if (auto itr = options.find(optionKey.data()); itr != options.end())
			{
				return itr->second;
			}
			return "";
		}

		std::unordered_map<std::string, std::string> options =
		{
			{ Reflect::Keys::Reflect_CMD_Option_PCH_Include, "" },
			{ Reflect::Keys::Reflect_CMD_Option_Reflect_Full_EXT, "false" },
			//{ Reflect::Reflect_CMD_Option_Single_File_EXT, "false" }

		};
	};
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