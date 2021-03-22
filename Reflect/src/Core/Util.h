#pragma once

#include "Core/Core.h"
#include <string>
#include <algorithm>

NAMESPACE_START

namespace Util
{
	static std::string ToLower(std::string str)
	{
		std::transform(str.begin(), str.end(), str.begin(), [](char c)
			{
				return std::tolower(c);
			});

		return str;
	}

	template<typename T>
	constexpr const char* GetTypeName()
	{
		return typeid(T).name();
	}

	template<typename T>
	constexpr const char* GetTypeName(const T& type)
	{
		return GetTypeName<T>();
	}
}

NAMESPACE_END