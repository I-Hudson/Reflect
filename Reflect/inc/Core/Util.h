#pragma once

#include "Core/Core.h"
#include "FileParser/FileParserKeyWords.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <typeinfo>
#include <vector>

#include <array>
#include <cstddef>
#include <cassert>

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
		std::string GetTypeName()
		{
			std::string name = typeid(T).name();
			for (const std::string& key : ContainerKeys)
			{
				RemoveString(name, key);
			}
			RemoveString(name, PointerTypeIdKey);
			RemoveCharAll(name, ' ');
			return name;
		}
		template<typename T>
		std::size_t GetTypeSize()
		{
			return sizeof(T);
		}

		template<typename T>
		std::string GetValueTypeName()
		{
			std::string name = GetTypeName<T>();
			RemoveCharAll(name, ReferenceKey);
			RemoveCharAll(name, PointerKey);
			return name;
		}
		template<typename T>
		std::size_t GetValueTypeSize()
		{
			return sizeof(std::remove_reference_t<std::remove_pointer_t<T>>);
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

		static void RemoveChar(std::string& str, const char c)
		{
			size_t index = str.find(c);
			std::string::iterator itr = str.begin() + (index != -1 ? index : str.size());
			str.erase(itr);
		}
		static void RemoveChar(std::string& str, const std::string c)
		{
			assert(c.size() == 1);
			RemoveChar(str, c.at(0));
		}

		static void RemoveCharAll(std::string& str, const char c)
		{
			str.erase(std::remove(str.begin(), str.end(), c), str.end());
		}
		static void RemoveCharAll(std::string& str, const std::string c)
		{
			assert(c.size() == 1);
			RemoveCharAll(str, c.at(0));
		}

		static void RemoveString(std::string& str, const std::string& remove, bool removeFromback = true)
		{
			size_t index = removeFromback ? str.rfind(remove) : str.find(remove);
			if (index != std::string::npos)
				str.erase(index, remove.size());
		}
		
		static bool StringContains(const std::string& str, const std::vector<char>& chars)
		{
			for (const char& strChar : str)
			{
				bool found = false;
				for (const char& cChars : chars)
				{
					if (strChar == cChars)
					{
						found = true;
						break;
					}
				}
				if (!found)
					return false;
			}
			return true;
		}

		static std::vector<std::string> SplitString(std::string str, char splitChar)
		{
			std::vector<std::string> strings;
			std::stringstream ss(str);
			str += '\n';
			while (ss.good())
			{
				std::string subStr;
				std::getline(ss, subStr, splitChar);
				strings.push_back(subStr);
			}
			return strings;
		}
	}
}
