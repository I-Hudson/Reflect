#pragma once

#include "Core/Core.h"
#include "Core/Enums.h"
#include <vector>
#include <functional>
#include <type_traits>
#include <unordered_map>

namespace Reflect 
{

struct ReflectTypeNameData
{
	std::string Type;
	std::string Name;
	int TypeSize;
	ReflectMemberType ReflectMemberType;
	bool IsConst;
	std::vector<ReflectFlags> ContainerProps;

	ReflectTypeNameData()
		: Type("Unkown")
		, Name("Unkown")
		, TypeSize(0)
		, ReflectMemberType(ReflectMemberType::Value)
		, IsConst(false)
	{ }

	ReflectTypeNameData(const std::string& type, const std::string& name, const int& typeSize, const Reflect::ReflectMemberType& memberType, const bool& isConst)
		: Type(type)
		, Name(name)
		, TypeSize(typeSize)
		, ReflectMemberType(memberType)
		, IsConst(isConst)
	{ }

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
	ReflectMemberProp(const char* name, const char* type, int offset, int properties)
		: Name(name)
		, Type(type)
		, Offset(offset)
		, Properties(properties)
	{ }

	const char* Name;
	const char* Type;
	int Offset;
	int Properties;
};

}
