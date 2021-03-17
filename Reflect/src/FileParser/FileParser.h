#pragma once

#include "Core/Core.h"
#include "Core/Enums.h"
#include <string>
#include <fstream>
#include <map>
#include <unordered_map>

NAMESPACE_START

struct ReflectMemberData
{
	std::string NativeType;
	std::string Name;
	int TypeSize;
	ReflectType Type = ReflectType::Member;
};

struct ReflectFunctionData
{
	ReflectType Type = ReflectType::Function;
};

struct RelfectData
{
	std::string Name;
	ReflectType Type;
	std::vector<ReflectContainerProps> ContainerProps;

	std::vector<ReflectMemberData> Members;
	std::vector<ReflectFunctionData> Functions;
};

struct FileParsedData
{
	std::string Data;
	int Cursor;
	RelfectData ReflectData;
};

/// <summary>
/// Parse a single file. This should extract all the info like functions and variables.
/// </summary>
class FileParser
{
public:
	FileParser();
	~FileParser();

	void ParseDirectory(const std::string& directory);
	std::ifstream OpenFile(const std::string& filePath);
	void CloseFile(std::ifstream& file);

private:
	bool CheckExtension(const std::string& filePath, std::vector<const char*> extensions);

	FileParsedData LoadFile(std::ifstream& file);
	
	void ParseFile(FileParsedData& fileData);
	bool ReflectContainer(FileParsedData& fileData, const std::string& keyword, const ReflectType type);
	bool RefectCheckForEndOfLine(const FileParsedData& fileData);
	bool ReflectTypeCheck(const std::string& type);

private:
	std::unordered_map<std::string, FileParsedData> m_filesParsed;
};

NAMESPACE_END