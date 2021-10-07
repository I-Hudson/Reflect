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

		static std::string Reverse(std::string str)
		{
			size_t strSize = str.size() - 1;
			for (size_t i = 0; i < str.size() * 0.5f; ++i)
			{
				size_t upperBound = strSize - i;
				char temp = str.at(i);
				str.at(i) = str.at(upperBound);
				str.at(upperBound) = temp;
			}
			return str;
		}

		static void RemoveChar(std::string& str, char c)
		{
			size_t index = str.find(c);
			std::string::iterator itr = str.begin() + (index != -1 ? index : str.size());
			str.erase(itr);
		}

		static void RemoveCharAll(std::string& str, char c)
		{
			str.erase(std::remove(str.begin(), str.end(), c));
		}

		static void RemoveString(std::string& str, const std::string& remove)
		{
			size_t index = str.find(remove);
			if (index != std::string::npos)
				str.erase(index, remove.size());
		}
	}
}
