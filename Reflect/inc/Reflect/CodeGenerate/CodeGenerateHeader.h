#pragma once

#include "Reflect/Core/Defines.h"
#include "Reflect/FileParser/ParserStructs.h"
#include "Reflect/CodeGenerate/CodeGenerate.h"

#include "Reflect/CodeGenerate/Headers/CG_Header_Legacy.h"
#include "Reflect/CodeGenerate/Headers/CG_Header_TypeInfo.h"

#include<fstream>

namespace Reflect
{
	struct ReflectAddtionalOptions;
}

namespace Reflect::CodeGeneration
{
	class CodeGenerateHeader
	{
	public:
		CodeGenerateHeader() { }
		~CodeGenerateHeader() { }

		void GenerateHeader(const Parser::FileParsedData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions);

	private:
		void WriteMacros(const Parser::FileParsedData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions);

	private:
		CG_Header_Legacy m_CGHeaderLegacy;
#ifdef REFLECT_TYPE_INFO_ENABLED
		CG_Header_TypeInfo m_CGHeaderTypeInfo;
#endif
	};
}
