#pragma once

#include "Core/Core.h"
#include <string>
#include <algorithm>

namespace Reflect
{
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

		std::string ValidateTypeName(const std::string& str);

		template<typename T>
		constexpr const char* GetTypeName()
		{
			return typeid(T).name();
		}

		template<typename T>
		constexpr std::string GetTypeName(const T& type)
		{
			return ValidateTypeName(GetTypeName<T>());
		}
	}
}
