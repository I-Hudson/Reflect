#pragma once

#include "Core/Core.h"
#include "ReflectStructs.h"

namespace Reflect 
{

class  CodeGenerateSource
{
public:
	CodeGenerateSource() { }
	~CodeGenerateSource() { }

	void GenerateSource(const FileParsedData& data, std::ofstream& file);

private:
	void WriteMemberProperties(const ReflectContainerData& data, std::ofstream& file);

	void WriteMemberGet(const ReflectContainerData& data, std::ofstream& file);
	void WriteFunctionGet(const ReflectContainerData& data, std::ofstream& file);
};

}
