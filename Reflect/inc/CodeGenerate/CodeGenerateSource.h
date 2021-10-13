#pragma once

#include "Core/Core.h"
#include "ReflectStructs.h"
#include "CodeGenerate/CodeGenerate.h"

namespace Reflect
{
	class CodeGenerateSource
	{
	public:
		CodeGenerateSource() { }
		~CodeGenerateSource() { }

		void GenerateSource(const FileParsedData& data, std::ofstream& file, const ReflectAddtionalOptions& addtionalOptions);

	private:
		
		void WriteMemberProperties(const ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions& addtionalOptions);

		void WriteMemberGet(const ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions& addtionalOptions);
		void WriteFunctionGet(const ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions& addtionalOptions);

		std::string MemberFormat();
	};
}
