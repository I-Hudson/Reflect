#pragma once

#include "Reflect/Core/Defines.h"
#include "Reflect/FileParser/ParserStructs.h"
#include "Reflect/CodeGenerate/CodeGenerate.h"

#include "Reflect/CodeGenerate/CPP/CG_CPP_Legacy.h"
#include "Reflect/CodeGenerate/CPP/CG_CPP_TypeInfo.h"

namespace Reflect
{
	struct ReflectAddtionalOptions;
}

namespace Reflect::CodeGeneration
{
	class CodeGenerateSource
	{
	public:
		CodeGenerateSource() { }
		~CodeGenerateSource() { }

		void GenerateSource(const Parser::FileParsedData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions);

	private:
		CG_CPP_Legacy m_CG_CPP_Legacy;
#ifdef REFLECT_TYPE_INFO_ENABLED
		CG_CPP_TypeInfo m_CG_CPP_TypeInfo;
#endif

	};
}
