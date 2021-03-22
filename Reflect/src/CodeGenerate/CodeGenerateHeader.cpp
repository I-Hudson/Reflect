#include "CodeGenerateHeader.h"
#include "CodeGenerate.h"
#include <assert.h>

NAMESPACE_START

std::string GetCurrentFileID(const std::string& fileName)
{
	return  fileName + "_Source_h";
}

#define WRITE_CURRENT_FILE_ID(FileName) file << "#define " + GetCurrentFileID(FileName)
#define WRITE_CLOSE() file << "\n\n"

#define WRITE_PUBLIC() file << "public:\\\n"
#define WRITE_PRIVATE() file << "private:\\\n"

void CodeGenerateHeader::GenerateHeader(const FileParsedData& data, std::ofstream& file)
{
	file << " // This file is auto generated please don't modify.\n";

	CodeGenerate::IncludeHeader("Core/ReflectObject.h", file);
	CodeGenerate::IncludeHeader("Core/Util.h", file);

	file << "\n";
	file << "#ifdef " + data.FileName + ReflectFileGeneratePrefix + "_h\n";
	file << "#error \"" + data.FileName + ReflectFileGeneratePrefix + ".h" + " already included, missing 'pragma once' in " + data.FileName + ".h\"\n";
	file << "#endif " + data.FileName + ReflectFileGeneratePrefix + "_h\n";
	file << "#define " + data.FileName + ReflectFileGeneratePrefix + "_h\n\n";

	WriteMacros(data, file);
}

void CodeGenerateHeader::WriteMacros(const FileParsedData& data, std::ofstream& file)
{
	for (const auto& reflectData : data.ReflectData)
	{
		const std::string CurrentFileId = GetCurrentFileID(data.FileName) + "_" + std::to_string(reflectData.ReflectGenerateBodyLine);

		WriteFunctionsMembers(reflectData, file, CurrentFileId);
		WriteMemberPropertiesOffsets(reflectData, file, CurrentFileId);
		WriteMemberGet(reflectData, file, CurrentFileId);

		WRITE_CURRENT_FILE_ID(data.FileName) + "_" + std::to_string(reflectData.ReflectGenerateBodyLine) + "_GENERATED_BODY \\\n";
		file << CurrentFileId + "_FUNCTION_MEMBERS \\\n";
		file << CurrentFileId + "_PROPERTIES_OFFSET \\\n";
		file << CurrentFileId + "_PROPERTIES_GET \\\n";

		WRITE_CLOSE();
	}

	file << "#undef CURRENT_FILE_ID\n";
	file << "#define CURRENT_FILE_ID " + GetCurrentFileID(data.FileName) + "\n";
}

void CodeGenerateHeader::WriteMemberPropertiesOffsets(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId)
{
	file << "#define " + currentFileId + "_PROPERTIES_OFFSET \\\n";
	WRITE_PRIVATE();
	for (const auto& member : data.Members)
	{
		file << "\tstatic int __REFLECT__" + member.Name + "() { return offsetof(" + data.Name + ", " + member.Name + "); }; \\\n";
	}
	WRITE_CLOSE();
}

void CodeGenerateHeader::WriteMemberGet(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId)
{
	file << "#define " + currentFileId + "_PROPERTIES_GET \\\n";
	WRITE_PUBLIC();
	file << "ReflectMember GetMember(const char* memberName);\\\n";
	WRITE_CLOSE();
}

void CodeGenerateHeader::WriteFunctionsMembers(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId)
{
	file << "#define " + currentFileId + "_FUNCTION_MEMBERS \\\n";
	WRITE_PRIVATE();
	for (const auto& func : data.Functions)
	{
		file << "\t" + func.Type + " __REFLECT_FUNC_MEMBER__" + func.Name + ";\\\n";
	}
	WRITE_CLOSE();
}

void CodeGenerateHeader::WriteFunctions(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId)
{
}

NAMESPACE_END