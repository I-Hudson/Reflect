#include "CodeGenerate.h"
#include <assert.h>
#include <filesystem>

NAMESPACE_START

constexpr const char* ContainerPrefix = "ReflectObject";

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
	source.GenerateSource(data, file);
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
	CodeGenerate::IncludeHeader("assert.h", file, true);

	for (auto& reflectData : data.ReflectData)
	{
		WriteHeader(reflectData, file);

		WriteBaseFunctions(reflectData, file);
		WriteOverrideFunctions(reflectData, file);

		WriteFooter(reflectData, file);
	}
}

void CodeGenerateSource::GenerateSource(const FileParsedData& data, std::ofstream& file)
{
	//CodeGenerate::IncludeHeader(data.FileName + ReflectFileGeneratePrefix + ".h", file);
	CodeGenerate::IncludeHeader("../" + data.FileName + ".h", file);

	for (auto& reflectData : data.ReflectData)
	{
		WriteConstructor(reflectData, file);
		WriteBaseFunctions(reflectData, file);
		WriteOverrideFunctions(reflectData, file);
	}
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
	file << "void " + data.Name + ContainerPrefix + "::" + "SetupReflectBindings()\n{\n";
	file << "\tif (m_reflectInit)\n";
	file << "\t{\n";
	file << "\t\treturn;\n";
	file << "\t}\n";
	file << "\tm_reflectInit = true;\n";
	file << "}\n\n";

	file << "ReflectFunction " + data.Name + ContainerPrefix + "::" + "GetFunction(const char* name)\n{\n";
	file << "\treturn __super::GetFunctionBase(name);\n";
	file << "}\n\n";
	file << "ReflectMember " + data.Name + ContainerPrefix + "::" + "GetMember(const char* name)\n{\n";
	file << "\treturn __super::GetMemberBase(name);\n";
	file << "}\n\n";
}

void CodeGenerateHeader::WriteBaseFunctions(const ReflectContainerData& data, std::ofstream& file)
{
	file << "\tvoid ReflectInit();\n";
}

void CodeGenerateHeader::WriteOverrideFunctions(const ReflectContainerData& data, std::ofstream& file)
{
	file << "\tvirtual const char* Verify() override { return \"" + data.Name + "\"; } \n";
	file << "\tvirtual ReflectFunction GetFunction(const char* name) override;\n";
	file << "\tvirtual ReflectMember GetMember(const char* name) override;\n";

	file << "protected:\n";
	file << "\tvirtual void SetupReflectBindings() override;\n";
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