#include "CodeGenerate/CodeGenerateSource.h"
#include "CodeGenerate/CodeGenerateHeader.h"
#include "CodeGenerate/CG_Utils.h"

#include "Core/Core.h"
#include "Instrumentor.h"

namespace Reflect::CodeGeneration
{
	void CodeGenerateSource::GenerateSource(const Parser::FileParsedData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();

		file << " // This file is auto generated by the Reflect tool please don't modify this file as it will be overwritten." << NEW_LINE;

		//CodeGenerate::IncludeHeader(data.FileName + ReflectFileGeneratePrefix + ".h", file);
		if (!additionalOptions->options.at(Keys::Reflect_CMD_Option_PCH_Include).empty())
		{
			CodeGenerate::IncludeHeader(additionalOptions->options.at(Keys::Reflect_CMD_Option_PCH_Include), file);
		}
		CodeGenerate::IncludeHeader("../" + data.FileName + ".h", file);
		file << NEW_LINE;

		for (const std::string& include : data.CPPIncludes)
		{
			CodeGenerate::IncludeHeader(include, file);
		}
		file << NEW_LINE;

		for (auto& reflectData : data.ReflectData)
		{
			CG_Utils::WriteIfDefines(reflectData, file);

#ifdef REFLECT_TYPE_INFO_ENABLED
			m_CG_CPP_TypeInfo.Generate(reflectData, file, additionalOptions);
#else
			m_CG_CPP_Legacy.Generate(reflectData, file, additionalOptions);
#endif
			CG_Utils::WriteEndIfDefines(reflectData, file);
		}
	}

	//void CodeGenerateSource::WriteFunctionBindings(const Parser::ReflectContainerData& data, std::ofstream& file)
	//{
	//	file << "\t" + data.Name << "* ptr = dynamic_cast<" + data.Name + "*>(this);" << NEW_LINE;
	//	file << "\tassert(ptr != nullptr && \"[" + data.Name + ContainerPrefix + "::" + "SetupReflectBindings()] 'ptr' should not be null.\");\n" << NEW_LINE;
	//	for (auto& func : data.Functions)
	//	{
	//		// Write to the auto generated file binding a function then setting up a "FuncWrapper" for which we can then call the function from.
	//		file << "\tauto " + func.Name + "Func = std::bind(&" + data.Name + "::" + func.Name + ", ptr);" << NEW_LINE;
	//		file << "\tReflect::FuncWrapper " + func.Name + "Wrapper(" + func.Name + "Func);" << NEW_LINE;
	//		file << "\t__super::AddFunction(" + func.Name + ", " + func.Name + "Wrapper);" << NEW_LINE;
	//	}
	//	file << NEW_LINE;
	//}
}
