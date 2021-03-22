#pragma once

#include "Core/Core.h"
#include "ReflectStructs.h"

NAMESPACE_START

class  CodeGenerateSource
{
public:
	CodeGenerateSource() { }
	~CodeGenerateSource() { }

	void GenerateSource(const FileParsedData& data, std::ofstream& file);

private:
	void WriteMemberGet(const ReflectContainerData& data, std::ofstream& file);

	//void WriteFunctionBindings(const ReflectContainerData& data, std::ofstream& file);
};

NAMESPACE_END