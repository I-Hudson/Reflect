#pragma once

#include "Core/Enums.h"

#include <string>
#include <vector>

namespace Reflect::Parser
{
	struct ReflectTypeNameData
	{
		std::string Type;
		std::string RawType;
		std::string Name;
		EReflectValueType ReflectValueType = EReflectValueType::Unknown;
		EReflectValueModifier ReflectModifier = EReflectValueModifier::None;
		std::vector<std::string> ContainerProps;

		std::vector<ReflectTypeNameData> TypeInheritance;

		int TypeSize;
		bool IsConst;

		ReflectTypeNameData()
			: Type("")
			, Name("")

			, TypeSize(0)
			, IsConst(false)
		{ }

		template<typename T>
		ReflectTypeNameData(const std::string& type, const std::string& rawType, const std::string& name, const int& typeSize, const Reflect::EReflectValueType& memberType, const bool& isConst)
			: Type(type)
			, Name(name)
			, RawType(rawType)
			, TypeSize(typeSize)
			, ReflectValueType(memberType)
			, IsConst(isConst)
		{ }

		bool operator!=(const ReflectTypeNameData& other) const
		{
			bool propsEqual = ContainerProps.size() != other.ContainerProps.size();
			if (propsEqual)
			{
				for (uint32_t i = 0; i < ContainerProps.size(); ++i)
				{
					if (ContainerProps[i] == other.ContainerProps[i])
					{
						propsEqual = false;
						break;
					}
				}
			}
			return Type != other.Type ||
				Name != other.Name ||
				TypeSize != other.TypeSize ||
				propsEqual;
		}
	};

	struct ReflectMemberData : public Parser::ReflectTypeNameData
	{
		EReflectType ReflectType = EReflectType::Member;
		int Offset = 0;
	};

	struct ReflectFunctionData : public Parser::ReflectTypeNameData
	{
		EReflectType ReflectType = EReflectType::Function;
		std::vector<ReflectTypeNameData> Parameters;
	};

	struct ReflectInheritanceData
	{
		std::string Name;
		std::string NameWithNamespace;
		bool IsReflected = true;
	};

	struct ReflectContainerData : public Parser::ReflectTypeNameData
	{
		std::string Name;
		std::string PrettyName;
		std::vector<ReflectInheritanceData> Inheritance;
		std::vector<std::string> Namespaces;
		std::string NameWithNamespace;
		EReflectType ReflectType;
		int ReflectGenerateBodyLine;

		std::vector<ReflectMemberData> Members;
		std::vector<Parser::ReflectFunctionData> Functions;
	};

	struct FileParsedData
	{
		std::string Data;
		int Cursor;
		std::string FilePath;
		std::string FileName;
		int GeneratedBodyLineOffset;
		std::vector<std::string> CPPIncludes;

		std::vector<Parser::ReflectContainerData> ReflectData;
		bool Parsed = false;
	};
}