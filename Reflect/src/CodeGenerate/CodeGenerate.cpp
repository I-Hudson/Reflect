#include "CodeGenerate.h"
#include <assert.h>
#include <filesystem>

NAMESPACE_START

constexpr const char* ContainerPrefix = "ReflectObject";

std::string GetCurrentFileID(const std::string& fileName, const std::string& extension)
{
	return  fileName + "_Source_" + extension;
}

CodeGenerate::CodeGenerate()
{ }

CodeGenerate::~CodeGenerate()
{ }

void CodeGenerate::Reflect(const FileParsedData& data)
{
	CodeGenerateHeader header;
	CodeGenerateSource source;

	if (!std::filesystem::exists(data.FilePath + "/Generated"))
	{
		std::filesystem::create_directory(data.FilePath + "/Generated");
	}

	std::ofstream file = OpenFile(data.FilePath + "/Generated/" + data.FileName + ReflectFileGeneratePrefix + ".h");
	header.GenerateHeader(data, file);
	CloseFile(file);

	file = OpenFile(data.FilePath + "/Generated/" + data.FileName + ReflectFileGeneratePrefix + ".cpp");
	//source.GenerateSource(data, file);
	CloseFile(file);
}

std::ofstream CodeGenerate::OpenFile(const std::string& filePath)
{
	std::ofstream file;
	file.open(filePath, std::ios::trunc);
	assert(file.is_open() && "[CodeGenerate::OpenFile] File could not be created.");
	return file;
}

void CodeGenerate::CloseFile(std::ofstream& file)
{
	if (file.is_open())
	{
		file.close();
	}
}

void CodeGenerateHeader::GenerateHeader(const FileParsedData& data, std::ofstream& file)
{
	file << " // This file is auto generated please don't modify.\n";

	CodeGenerate::IncludeHeader("Core/ReflectObject.h", file);

	file << "\n";
	file << "#ifdef " + data.FileName + ReflectFileGeneratePrefix + "_h\n";
	file << "#error \"" + data.FileName + ReflectFileGeneratePrefix + ".h" + " already included, missing 'pragma once' in " + data.FileName + ".h\"\n";
	file << "#endif " + data.FileName + ReflectFileGeneratePrefix + "_h\n";
	file << "#define " + data.FileName + ReflectFileGeneratePrefix + "_h\n\n";

	WriteMacros(data, file);

	//for (auto& reflectData : data.ReflectData)
	//{
	//	WriteHeader(reflectData, file);
	//
	//	WriteBaseFunctions(reflectData, file);
	//	WriteOverrideFunctions(reflectData, file);
	//
	//	WriteFooter(reflectData, file);
	//}
}

void CodeGenerateSource::GenerateSource(const FileParsedData& data, std::ofstream& file)
{
	//CodeGenerate::IncludeHeader(data.FileName + ReflectFileGeneratePrefix + ".h", file);
	CodeGenerate::IncludeHeader("../" + data.FileName + ".h", file);
	CodeGenerate::IncludeHeader("assert.h", file, true);
	CodeGenerate::IncludeHeader("functional", file, true);

	for (auto& reflectData : data.ReflectData)
	{
		WriteConstructor(reflectData, file);
		WriteBaseFunctions(reflectData, file);
		WriteOverrideFunctions(reflectData, file);
	}
}

void CodeGenerateHeader::WriteMacros(const FileParsedData& data, std::ofstream& file)
{
	for (const auto& reflectData :data.ReflectData)
	{
		const std::string CurrentFileId = GetCurrentFileID(data.FileName, "h") + "_" + std::to_string(reflectData.ReflectGenerateBodyLine);
		WriteMemberProperties(reflectData, file, CurrentFileId);
		WriteMemberGet(reflectData, file, CurrentFileId);

		file << "#define " + CurrentFileId + "_GENERATED_BODY \\\n";
		file << CurrentFileId + "_PROPERTIES_OFFSET \\\n";
		file << CurrentFileId + "_PROPERTIES_GET";

		file << "\n\n";
	}

	file << "#undef CURRENT_FILE_ID\n";
	file << "#define CURRENT_FILE_ID " + GetCurrentFileID(data.FileName, "h") + "\n";
}

void CodeGenerateHeader::WriteMemberProperties(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId)
{
	file << "#define " + currentFileId + "_PROPERTIES_OFFSET \\\n";
	for (const auto& member :data.Members)
	{
		file << "\tstatic int __REFLECT__" + member.Name + "() { return offsetof(" + data.Name + ", " + member.Name + "); }; \\\n";
	}
	file << "\n";
}

void CodeGenerateHeader::WriteMemberGet(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId)
{
	file << "#define " + currentFileId + "_PROPERTIES_GET \\\n";
	file << "public:\\\n";
	file << "ReflectMember GetMember(const char* memberName)\\\n{\\\n";
	for (const auto& member : data.Members)
	{
		file << "\tif(memberName == \"" + member.Name + "\")\\\n";
		file << "\t{\\\n";
		file << "\t\treturn ReflectMember(&" + member.Name + ");\\\n";
		file << "\t}\\\n";
	}
	file << "}\\\nprivate:\n\n";
}

void CodeGenerateHeader::WriteHeader(const ReflectContainerData& data, std::ofstream& file)
{
	if (data.ReflectType == ReflectType::Struct)
	{
		file << "struct ";
	}
	else if (data.ReflectType == ReflectType::Class)
	{
		file << "class ";
	}
	else
	{
		assert(false && "[CodeGenerate::WriteHeader] Container type not supported.");
	}

	file << data.Name + ContainerPrefix + " : ReflectObject" +
		"\n{\npublic:\n\t" +
		data.Name + ContainerPrefix + "();\n" +
		"\t~" + data.Name + ContainerPrefix + "();\n";
}

void CodeGenerateHeader::WriteFooter(const ReflectContainerData& data, std::ofstream& file)
{
	file << "};\n\n";
}

void CodeGenerateSource::WriteConstructor(const ReflectContainerData& data, std::ofstream& file)
{
	file << data.Name + ContainerPrefix + "::" + data.Name + ContainerPrefix + "()\n{";
	file << "}\n\n";
	file << data.Name + ContainerPrefix + "::~" + data.Name + ContainerPrefix + "()\n{";
	file << "}\n\n";
}

void CodeGenerateSource::WriteBaseFunctions(const ReflectContainerData& data, std::ofstream& file)
{
	file << "void " + data.Name + ContainerPrefix + "::" + "ReflectInit()\n{";
	file << "}\n\n";
}

void CodeGenerateSource::WriteOverrideFunctions(const ReflectContainerData& data, std::ofstream& file)
{
	//file << "void " + data.Name + ContainerPrefix + "::" + "SetupReflectBindings()\n{\n";
	//file << "\tif (m_reflectInit)\n";
	//file << "\t{\n";
	//file << "\t\treturn;\n";
	//file << "\t}\n";
	//file << "\tm_reflectInit = true;\n";
	////WriteFunctionBindings(data, file);
	//file << "}\n\n";

	//file << "ReflectFunction " + data.Name + ContainerPrefix + "::" + "GetFunction(const char* name)\n{\n";
	////file << "\treturn ReflectFunction();\n";
	//file << "\t" + data.Name << "* ptr = dynamic_cast<" + data.Name + "*>(this);\n";
	//file << "\tassert(ptr != nullptr && \"[" + data.Name + ContainerPrefix + "::" + "GetFunction] 'ptr' should not be null.\");\n\n";
	//for (auto& func : data.Functions)
	//{
	//	file << "\tif (\"" + func.Name + "\")\n\t{\n";
	//	file << "\t\tauto " + func.Name + "Func = std::bind(&" + data.Name + "::" + func.Name + ", ptr);\n";
	//	file << "\t\tReflect::FuncWrapper " + func.Name + "Wrapper(" + func.Name + "Func);\n";
	//	file << "\t}\n";
	//}
	//file << "}\n\n";

	file << "ReflectMember " + data.Name + ContainerPrefix + "::" + "GetMember(const char* name)\n{\n";
	file << "\treturn ReflectMember();\n";
	file << "}\n\n";
}

void CodeGenerateSource::WriteFunctionBindings(const ReflectContainerData& data, std::ofstream& file)
{
	file << "\t" + data.Name << "* ptr = dynamic_cast<" + data.Name + "*>(this);\n";
	file << "\tassert(ptr != nullptr && \"[" + data.Name + ContainerPrefix + "::" + "SetupReflectBindings()] 'ptr' should not be null.\");\n\n";
	for (auto& func : data.Functions)
	{
		// Write to the auto generated file binding a function then setting up a "FuncWrapper" for which we can then call the function from.
		file << "\tauto " + func.Name + "Func = std::bind(&" + data.Name + "::" + func.Name + ", ptr);\n";
		file << "\tReflect::FuncWrapper " + func.Name + "Wrapper(" + func.Name + "Func);\n";
		file << "\t__super::AddFunction(" + func.Name + ", " + func.Name + "Wrapper);\n";
	}
	file << "\n";
}

void CodeGenerateHeader::WriteBaseFunctions(const ReflectContainerData& data, std::ofstream& file)
{
	file << "\tvoid ReflectInit();\n";
	file << "\tauto GetFunction(const char* name)\n\t{\n";
	file << "\t\t" + data.Name << "* ptr = dynamic_cast<" + data.Name + "*>(this);\n";
	file << "\t\tassert(ptr != nullptr && \"[" + data.Name + ContainerPrefix + "::" + "GetFunction] 'ptr' should not be null.\");\n\n";

	bool first = true;
	for (const auto& func : data.Functions)
	{
		file << "\t\t";
		if (!first)
		{
			file << "else";
		}
		file << "REFLECT_GET_FUNCTION(" + data.Name + ", " + func.Name + ", ptr)\n";
		first = false;
	}
	file << "\t\tassert(false && \"\");\n";
	file << "\t}\n";
	file << "\tReflectMember GetMember(const char* name);\n";
}

void CodeGenerateHeader::WriteOverrideFunctions(const ReflectContainerData& data, std::ofstream& file)
{
	file << "\tvirtual const char* Verify() override { return \"" + data.Name + "\"; } \n";

	file << "protected:\n";
	//file << "\tvirtual void SetupReflectBindings() override;\n";
}

void CodeGenerate::IncludeHeader(const std::string& headerToInclude, std::ofstream& file, bool windowsInclude)
{
	if (windowsInclude)
	{
		file << "#include <" + headerToInclude + ">\n";
	}
	else
	{
		file << "#include \"" + headerToInclude + "\"\n";
	}
}

NAMESPACE_END