#pragma once

#include "Reflect/FileParser/FileParserKeyWords.h"
#include "Reflect/Core/Enums.h"

#include <string>
#include <algorithm>
#include <vector>

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
			for (const std::string& key : Keys::ContainerKeys)
			{
				RemoveString(name, key);
			}
			RemoveString(name, Keys::PointerTypeIdKey);
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
			RemoveCharAll(name, Keys::ReferenceKey);
			RemoveCharAll(name, Keys::PointerKey);
			return name;
		}
		template<typename T>
		std::size_t GetValueTypeSize()
		{
			return sizeof(std::remove_reference_t<std::remove_pointer_t<T>>);
		}

		static std::string RemoveNamespaces(const std::string& string)
		{
			std::string str = string;
			size_t namespaceChar = string.find_last_of("::");
			if (namespaceChar != std::string::npos)
			{
				str = str.substr(namespaceChar + 1);
			}
			return str;
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

		static void RemoveChar(std::string& str, const char& c)
		{
			size_t index = str.find(c);
			std::string::iterator itr = str.begin() + (index != -1 ? index : str.size());
			str.erase(itr);
		}
		static void RemoveCharReverse(std::string& str, const char& c)
		{
			size_t index = str.rfind(c);
			std::string::iterator itr = str.begin() + (index != -1 ? index : str.size());
			str.erase(itr);
		}

		static void RemoveCharAll(std::string& str, const char& c)
		{
			str.erase(std::remove(str.begin(), str.end(), c), str.end());
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
			std::string temp;
			temp.reserve(str.size());

			for (size_t i = 0; i < str.size(); ++i)
			{
				if (str[i] == splitChar)
				{
					strings.push_back(temp);
					temp = "";
				}
				else
				{
					temp += str[i];
				}
			}
			if (!temp.empty())
			{
				strings.push_back(temp);
			}

			return strings;
		}
		static std::vector<std::string_view> SplitStringView(std::string_view str, char splitChar)
		{
			std::vector<std::string_view> strings;
			std::string_view temp;
			uint64_t strStart = 0;
			uint64_t strEnd = 0;

			for (size_t i = 0; i < str.size(); ++i)
			{
				if (str[i] == splitChar)
				{
					strings.push_back(str.substr(strStart, strEnd - strStart));
					++strEnd;
					strStart = strEnd;
				}
				else
				{
					++strEnd;
				}
			}
			if (strStart != strEnd)
			{
				strings.push_back(str.substr(strStart, strEnd - strStart));
			}

			return strings;
		}

		static std::string EReflectValueTypeToString(EReflectValueType mod)
		{
			switch (mod)
			{
			case Reflect::EReflectValueType::Value:
				return "";
			case Reflect::EReflectValueType::Reference:
				return "&";
			case Reflect::EReflectValueType::Pointer:
				return "*";
			case Reflect::EReflectValueType::PointerReference:
				return "*&";
			default:
				break;
			}
			return "";
		}
	}
}
