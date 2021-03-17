#pragma once

#include "Core/Core.h"
#include <string>
#include <fstream>
#include <map>
#include <unordered_map>

NAMESPACE_START

struct FileParsedData
{
	std::string Data;
	int Cursor;
};

/// <summary>
/// Parse a single file. This should extract all the info like functions and variables.
/// </summary>
class FileParser
{
public:
	FileParser();
	~FileParser();

	void ParseDirectory(const std::string& dir);
	std::ifstream OpenFile(const std::string& filePath);
	void CloseFile(std::ifstream& file);

private:
	bool CheckExtension(const std::string& filePath, std::vector<const char*> extensions);

	FileParsedData LoadFile(std::ifstream& file);
	void ParseFile(std::ifstream& file);
	void ParseLine(const std::string& line);

	void ExtractReflectType(const std::string& line);
	void ExtractReturnType(const std::string& line);
	void ExtractName(const std::string& line);

	void ExtractParameters(const std::string& line);

private:
	std::unordered_map<std::string, FileParsedData> m_filesParsed;
};

NAMESPACE_END