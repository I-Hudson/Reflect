#pragma once

#include "Core/Core.h"
#include "ReflectStructs.h"

#include<fstream>

NAMESPACE_START

/// <summary>
/// Generate a new file and place the reflect code there.
/// </summary>
class CodeGenerate
{
public:
	CodeGenerate();
	~CodeGenerate();

	void Reflect(const FileParsedData& data);

private:
	std::ofstream OpenFile(const std::string& filePath);
	void CloseFile(std::ofstream& file);

	void WriteHeader(const ReflectContainerData& data, std::ofstream& file);
	void WriteFooter(const ReflectContainerData& data, std::ofstream& file);

	void WriteOverrideFunctions(const ReflectContainerData& data, std::ofstream& file);

private:
};

NAMESPACE_END