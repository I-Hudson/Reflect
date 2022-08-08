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

#ifdef REFLET_TYPE_INFO
		void WriteGenerateTypeInfo(const ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions& addtionalOptions);
#endif

		std::string MemberFormat();
	};
}
