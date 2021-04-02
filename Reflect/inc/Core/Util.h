#pragma once

#include "Core/Core.h"
#include <string>
#include <algorithm>
#include <typeinfo>

namespace Reflect
{
	namespace Util
	{
		static std::string ToLower(std::string str)
		{
			std::transform(str.begin(), str.end(), str.begin(), [](char c)
			{
				return static_cast<char>(std::tolower(static_cast<int>(c)));
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
		std::string GetTypeName(const T& type)
		{
			return ValidateTypeName(GetTypeName<T>());
		}
	}
}
