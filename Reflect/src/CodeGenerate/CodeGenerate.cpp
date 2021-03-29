#include "CodeGenerate/CodeGenerate.h"
#include "CodeGenerate/CodeGenerateHeader.h"
#include "CodeGenerate/CodeGenerateSource.h"
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