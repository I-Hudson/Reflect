#include "CodeGenerate/Headers/CG_Header_TypeInfo.h"
#include "CodeGenerate/Headers/CG_Header_Legacy.h"
#include "CodeGenerate/CodeGenerate.h"

#include "Core/Core.h"
#include "Instrumentor.h"

namespace Reflect
{
#ifdef REFLECT_TYPE_INFO_ENABLED
	void CG_Header_TypeInfo::WriteGenerateTypeInfo(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		file << "#define " + currentFileId + "_GENERATE_TYPE_INFO" + NEW_LINE_SLASH;
		WRITE_PUBLIC;
		file << TAB << "static Reflect::ReflectTypeInfo GetTypeInfo();" << NEW_LINE_SLASH;
		file << TAB << "static Reflect::ReflectTypeInfo GetTypeInfo(" + data.Name + "* classPtr);" + NEW_LINE_SLASH;
		WRITE_PRIVATE;
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