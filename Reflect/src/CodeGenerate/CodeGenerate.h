#pragma once

#include "Core/Core.h"
#include "ReflectStructs.h"

#include<fstream>

NAMESPACE_START

class CodeGenerateHeader
{
public:
	CodeGenerateHeader() { }
	~CodeGenerateHeader() { }

	void GenerateHeader(const FileParsedData& data, std::ofstream& file);

private:
	void WriteMacros(const FileParsedData& data, std::ofstream& file);
	void WriteMemberProperties(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId);
	void WriteMemberGet(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId);

	void WriteHeader(const ReflectContainerData& data, std::ofstream& file);
	void WriteFooter(const ReflectContainerData& data, std::ofstream& file);

	void WriteBaseFunctions(const ReflectContainerData& data, std::ofstream& file);
	void WriteOverrideFunctions(const ReflectContainerData& data, std::ofstream& file);
};

class  CodeGenerateSource
{
public:
	CodeGenerateSource() { }
	~CodeGenerateSource() { }

	void GenerateSource(const FileParsedData& data, std::ofstream& file);

private:
	void WriteConstructor(const ReflectContainerData& data, std::ofstream& file);
	void WriteBaseFunctions(const ReflectContainerData& data, std::ofstream& file);
	void WriteOverrideFunctions(const ReflectContainerData& data, std::ofstream& file);

	void WriteFunctionBindings(const ReflectContainerData& data, std::ofstream& file);
};

/// <summary>
/// Generate a new file and place the reflect code there.
/// </summary>
class CodeGenerate
{
public:
	CodeGenerate();
	~CodeGenerate();

	void Reflect(const FileParsedData& data);
	static void IncludeHeader(const std::string& headerToInclude, std::ofstream& file, bool windowsInclude = false);

private:
	std::ofstream OpenFile(const std::string& filePath);
	void CloseFile(std::ofstream& file);
};

NAMESPACE_END