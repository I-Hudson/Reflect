#pragma once

#include "Core/Core.h"
#include "Core/Enums.h"
#include <vector>

NAMESPACE_START

struct ReflectTypeNameData
{
	std::string Type;
	std::string Name;
	int TypeSize;
	std::vector<ReflectFlags> ContainerProps;
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

	std::vector<ReflectMemberData> Members;
	std::vector<ReflectFunctionData> Functions;
};

struct FileParsedData
{
	std::string Data;
	int Cursor;

	std::vector<ReflectContainerData> ReflectData;
};

NAMESPACE_END