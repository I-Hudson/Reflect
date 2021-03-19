#include "CodeGenerate.h"
#include <assert.h>

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

	std::ofstream file = OpenFile(data.FilePath + "/" + data.FileName + ReflectFileGeneratePrefix + ".h");
	header.GenerateHeader(data, file);
	CloseFile(file);

	file = OpenFile(data.FilePath + "/" + data.FileName + ReflectFileGeneratePrefix + ".cpp");
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

		WriteOverrideFunctions(reflectData, file);

		WriteFooter(reflectData, file);
	}
}

void CodeGenerateSource::GenerateSource(const FileParsedData& data, std::ofstream& file)
{
	CodeGenerate::IncludeHeader(data.FileName + ReflectFileGeneratePrefix + ".h", file);
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

	file << data.Name + ContainerPrefix +" : public ReflectObject" +
		"\n{\n\t" +
		data.Name + ContainerPrefix + "();\n";
}

void CodeGenerateHeader::WriteFooter(const ReflectContainerData& data, std::ofstream& file)
{
	file << "};\n\n";
}

void CodeGenerateSource::WriteConstructor(const ReflectContainerData& data, std::ofstream& file)
{
	file << "\t\tint x = 10;\n";
}

void CodeGenerateHeader::WriteOverrideFunctions(const ReflectContainerData& data, std::ofstream& file)
{
	file << "\tvirtual const char* Verify() override { return \"" + data.Name + "\"; } \n";
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