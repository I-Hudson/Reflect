#pragma once

#include "Core/Core.h"
#include "ReflectStructs.h"
#include<fstream>

namespace Reflect
{

class CodeGenerateHeader
{
public:
	CodeGenerateHeader() { }
	~CodeGenerateHeader() { }

	void GenerateHeader(const FileParsedData& data, std::ofstream& file);

private:
	void WriteMacros(const FileParsedData& data, std::ofstream& file);
	void WriteMemberProperties(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId);
	void WriteMemberPropertiesOffsets(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId);
	void WriteMemberGet(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId);

	void WriteFunctions(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId);
	void WriteFunctionGet(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId);
};

}
