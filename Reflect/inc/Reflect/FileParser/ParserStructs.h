#pragma once

#include "Reflect/Core/Enums.h"

#include "Reflect/Structs/PropertyMeta.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace Reflect::Parser
{
	struct ReflectInheritanceData
	{
		std::string Name;
		std::string NameWithNamespace;
		bool IsParsed = false;
		bool HasCodeGenerated = false;

		std::vector<ReflectInheritanceData> Inheritances;

		bool InheritsFromType(std::string_view type) const
		{
			for (const ReflectInheritanceData& data : Inheritances)
			{
				if (data.Name == type)
				{
					return true;
				}
			}

			for (const ReflectInheritanceData& data : Inheritances)
			{
				if (data.InheritsFromType(type))
				{
					return true;
				}
			}

			return false;
		}

		bool InheritsFromTypeWithNamespaces(std::string_view typeWithNamespaces) const
		{
			for (const ReflectInheritanceData& data : Inheritances)
			{
				if (data.NameWithNamespace == typeWithNamespaces)
				{
					return true;
				}
			}

			for (const ReflectInheritanceData& data : Inheritances)
			{
				if (data.InheritsFromType(typeWithNamespaces))
				{
					return true;
				}
			}

			return false;
		}
	};

	struct ReflectTemplateData
	{
		ReflectTemplateData(std::string type, std::string identifier)
			: Type(type)
			, Identifier(identifier)
		{ }
		ReflectTemplateData(std::string type, std::string identifier, std::string defaultValue)
			: Type(type)
			, Identifier(identifier)
			, DefaultValue(defaultValue)
		{ }

		std::string Type;
		std::string Identifier;
		std::string DefaultValue;

		bool HasDefualtValue() const { return DefaultValue.empty(); }
	};

	struct ReflectIfDefineData
	{
		std::string Data;
		uint64_t Line;
	};

	struct ReflectTypeNameData
	{
		std::string Type;
		std::string RawType;
		std::string Name;
		std::string NameWithNamespace;
		EReflectValueType ReflectValueType = EReflectValueType::Unknown;
		EReflectValueModifier ReflectModifier = EReflectValueModifier::None;
		std::vector<std::string> ContainerProps;
		std::vector<PropertyMeta> PropertyMetas;
		std::vector<ReflectIfDefineData> IfDefines;

		std::vector<ReflectInheritanceData> TypeInheritance;

		int TypeSize;
		bool IsConst;
		bool IsTemplate = false;
		std::vector<ReflectTemplateData> TemplateTypes;
		std::string TemplateFirstType;
		std::string TemplateSecondType;

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
		size_t Offset = 0;
	};

	struct ReflectFunctionData : public Parser::ReflectTypeNameData
	{
		EReflectType ReflectType = EReflectType::Function;
		std::vector<ReflectTypeNameData> Parameters;
	};

	struct ReflectContainerData : public Parser::ReflectTypeNameData
	{
		std::string Name;
		std::string PrettyName;
		std::vector<ReflectInheritanceData> Inheritance;
		std::vector<std::string> Namespaces;
		std::vector<ReflectIfDefineData> IfDefines;
		EReflectType ReflectType;
		size_t ReflectGenerateBodyLine;

		std::vector<ReflectMemberData> Members;
		std::vector<Parser::ReflectFunctionData> Functions;

		bool GenerateReflectCode = false;
		bool HasBeenParsed = false;
	};

	struct FileParserOptions
	{
		/// @brief Mark all files to not be reflected. To be used if you want to parser a directory to get 
		/// relevant data for another directoy.
		bool DoNotReflect = false;
	};

	struct FileParsedData
	{
		std::string Data;
		size_t Cursor;
		std::string FullPath;
		std::string FilePath;
		std::string FileName;
		size_t GeneratedBodyLineOffset;
		std::vector<std::string> CPPIncludes;

		std::vector<Parser::ReflectContainerData> ReflectData;
		bool Parsed = false;
		// If true. This something has gone wrong in which we can't recover from. If true then all relfect data for this file
		// will be removed.
		bool AbortParsing = false;
		FileParserOptions parserOptions;
	};
}