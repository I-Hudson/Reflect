#include "CodeGenerate.h"
#include <assert.h>

NAMESPACE_START

CodeGenerate::CodeGenerate()
{ }

CodeGenerate::~CodeGenerate()
{ }

void CodeGenerate::Reflect(const FileParsedData& data)
{
	std::ofstream file = OpenFile(data.FilePath + "/" + data.FileName + ReflectFileGeneratePrefix + ".h");
	file << " // This file is auto generated please don't modify. \n #include \"Core/ReflectObject.h\" \n\n";

	for (auto& reflectData : data.ReflectData)
	{
		WriteHeader(reflectData, file);

		WriteOverrideFunctions(reflectData, file);

		WriteFooter(reflectData, file);
	}

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

void CodeGenerate::WriteHeader(const ReflectContainerData& data, std::ofstream& file)
{
	if (data.ReflectType == ReflectType::Struct)
	{
		file << "struct " + data.Name + "ReflectObject : public ReflectObject" + "\n{\n";
	}
	else if (data.ReflectType == ReflectType::Class)
	{
		file << "class " + data.Name + "ReflectObject : public ReflectObject" + "\n{\n";
	}
	else
	{
		assert(false && "[CodeGenerate::WriteHeader] Container type not supported.");
	}
}

void CodeGenerate::WriteFooter(const ReflectContainerData& data, std::ofstream& file)
{
	file << "}; \n\n";
}

void CodeGenerate::WriteOverrideFunctions(const ReflectContainerData& data, std::ofstream& file)
{
	file << "virtual const char* Verify() override { return \"" + data.Name + "\"; } \n";
}

NAMESPACE_END