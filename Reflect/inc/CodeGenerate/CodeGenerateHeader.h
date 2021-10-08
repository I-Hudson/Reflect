#pragma once

#include "Core/Core.h"
#include "ReflectStructs.h"
#include "CodeGenerate/CodeGenerate.h"
#include<fstream>

namespace Reflect
{
	class CodeGenerateHeader
	{
	public:
		CodeGenerateHeader() { }
		~CodeGenerateHeader() { }

		void GenerateHeader(const FileParsedData& data, std::ofstream& file, const CodeGenerateAddtionalOptions& addtionalOptions);

	private:
		void WriteMacros(const FileParsedData& data, std::ofstream& file, const CodeGenerateAddtionalOptions& addtionalOptions);
		void WriteMemberProperties(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const CodeGenerateAddtionalOptions& addtionalOptions);
		void WriteMemberPropertiesOffsets(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const CodeGenerateAddtionalOptions& addtionalOptions);
		void WriteMemberGet(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const CodeGenerateAddtionalOptions& addtionalOptions);

		void WriteFunctions(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const CodeGenerateAddtionalOptions& addtionalOptions);
		void WriteFunctionGet(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const CodeGenerateAddtionalOptions& addtionalOptions);

		std::string GetType(const Reflect::ReflectTypeNameData& arg);
	};
}
