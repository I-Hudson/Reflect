#pragma once

#include "Core/Defines.h"
#include "ReflectStructs.h"
#include "CodeGenerate/CodeGenerate.h"

#include "CodeGenerate/CPP/CG_CPP_Legacy.h"
#include "CodeGenerate/CPP/CG_CPP_TypeInfo.h"

namespace Reflect
{
	struct ReflectAddtionalOptions;

	class CodeGenerateSource
	{
	public:
		CodeGenerateSource() { }
		~CodeGenerateSource() { }

		void GenerateSource(const FileParsedData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions);

	private:
		CG_CPP_Legacy m_CG_CPP_Legacy;
#ifdef REFLECT_TYPE_INFO_ENABLED
		CG_CPP_TypeInfo m_CG_CPP_TypeInfo;
#endif

	};
}
