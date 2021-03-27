#pragma once

#include "Core/Core.h"
#include "Core/Enums.h"
#include <vector>
#include <functional>
#include <type_traits>
#include <unordered_map>

NAMESPACE_START

struct ReflectTypeNameData
{
	std::string Type;
	std::string Name;
	int TypeSize;
	ReflectMemberType ReflectMemberType;
	std::vector<ReflectFlags> ContainerProps;

	bool operator!=(const ReflectTypeNameData& other) const
	{
		bool propsEqual = ContainerProps.size() != other.ContainerProps.size();
		if (propsEqual)
		{
			int i = 0;
			for (const auto& prop : ContainerProps)
			{
				if (prop == other.ContainerProps[i])
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

struct ReflectMemberData : public ReflectTypeNameData
{
	ReflectType ReflectType = ReflectType::Member;
};

struct ReflectFunctionData : public ReflectTypeNameData
{
	ReflectType ReflectType = ReflectType::Function;
	std::vector<ReflectTypeNameData> Parameters;
};

struct ReflectContainerData : public ReflectTypeNameData
{
	std::string Name;
	ReflectType ReflectType;
	int ReflectGenerateBodyLine;

	std::vector<ReflectMemberData> Members;
	std::vector<ReflectFunctionData> Functions;
};

struct FileParsedData
{
	std::string Data;
	int Cursor;
	std::string FilePath;
	std::string FileName;
	int GeneratedBodyLineOffset;

	std::vector<ReflectContainerData> ReflectData;
};

struct ReflectMemberProp
{
	ReflectMemberProp(const char* name, int offset, int properties)
		: Name(name)
		, Offset(offset)
		, Properties(properties)
	{ }

	const char* Name;
	int Offset;
	int Properties;
};

NAMESPACE_END