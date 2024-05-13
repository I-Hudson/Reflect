#include "Reflect/CodeGenerate/Headers/CG_Header_TypeInfo.h"
#include "Reflect/CodeGenerate/Headers/CG_Header_Legacy.h"
#include "Reflect/CodeGenerate/CodeGenerate.h"

#include "Reflect/Core/Core.h"
#include "Reflect/Instrumentor.h"

namespace Reflect::CodeGeneration
{
#ifdef REFLECT_TYPE_INFO_ENABLED
	void CG_Header_TypeInfo::WriteGenerateTypeInfo(const Parser::ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		file << "#define " + currentFileId + "_GENERATE_TYPE_INFO" + NEW_LINE_SLASH;
		WRITE_PUBLIC;
		file << TAB << "static Reflect::TypeInfo GetStaticTypeInfo();" << NEW_LINE_SLASH;
		file << TAB << "static Reflect::TypeInfo GetStaticTypeInfo(" + data.NameWithNamespace + "* ownerPointer);" << NEW_LINE_SLASH;
		file << TAB << "virtual Reflect::TypeInfo GetTypeInfo();" << NEW_LINE_SLASH;
		WRITE_PRIVATE;
		file << TAB << "static Reflect::TypeInfoRegister s_TypeInfoRegister;" << NEW_LINE_SLASH;
		file << TAB << "static Reflect::TypeInfo ReflectRegisterCallback(void* objectInstance);" << NEW_LINE_SLASH;
		file << TAB << "friend class Reflect::GenerateTypeInfoForType<" + data.Name + ">;";
		file << NEW_LINE << NEW_LINE;
		CG_Header_Legacy::WriteFunctions(data, file, currentFileId, additionalOptions);
		WRITE_CLOSE;
	}

	void CG_Header_TypeInfo::WriteClosingMacro(std::ofstream& file, std::string_view currentFileId)
	{
		REFLECT_PROFILE_FUNCTION();
		file << currentFileId;
		file << "_GENERATE_TYPE_INFO";
		file<< NEW_LINE_SLASH;
	}
#endif
}