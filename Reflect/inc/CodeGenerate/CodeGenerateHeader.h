#pragma once

#include "Core/Defines.h"
#include "ReflectStructs.h"
#include "CodeGenerate/CodeGenerate.h"

#include "CodeGenerate/Headers/CG_Header_Legacy.h"
#include "CodeGenerate/Headers/CG_Header_TypeInfo.h"

#include<fstream>

namespace Reflect
{
	struct ReflectAddtionalOptions;

	class CodeGenerateHeader
	{
	public:
		CodeGenerateHeader() { }
		~CodeGenerateHeader() { }

		void GenerateHeader(const FileParsedData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions);

	private:
		void WriteMacros(const FileParsedData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions);

	private:
		CG_Header_Legacy m_CGHeaderLegacy;
#ifdef REFLECT_TYPE_INFO_ENABLED
		CG_Header_TypeInfo m_CGHeaderTypeInfo;
#endif
	};
}
