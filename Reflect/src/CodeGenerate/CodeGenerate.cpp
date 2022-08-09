#include "CodeGenerate/CodeGenerate.h"
#include "CodeGenerate/CodeGenerateHeader.h"
#include "CodeGenerate/CodeGenerateSource.h"
#include "Instrumentor.h"
#include <assert.h>
#include <filesystem>

namespace Reflect
{
	constexpr const char* ContainerPrefix = "ReflectObject";

	CodeGenerate::CodeGenerate()
	{ }

	CodeGenerate::~CodeGenerate()
	{ }

	void CodeGenerate::Reflect(const FileParser& parser, const ReflectAddtionalOptions& addtionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();

		CodeGenerateHeader header;
		CodeGenerateSource source;

#ifdef REFLECT_SINGLE_FILE
		//if (addtionalOptions.GetOption(Reflect::Reflect_CMD_Option_Single_File_EXT) == "true")
		{
			const std::string dir = parser.GetDirectoryParsed(0);
			if (!std::filesystem::exists(dir + "/Generated"))
			{
				std::filesystem::create_directory(dir + "/Generated");
			}

			constexpr const char* fileName = "ReflectData";

			std::ofstream fileHeader = OpenFile(dir + "/Generated/" + fileName + ReflectFileGeneratePrefix + ".h");
			std::ofstream fileCPP = OpenFile(dir + "/Generated/" + fileName + ReflectFileGeneratePrefix + ".cpp");

			for (const auto& data : parser.GetAllFileParsedData())
			{
				header.GenerateHeader(data, fileHeader, addtionalOptions);
				source.GenerateSource(data, fileCPP, addtionalOptions);
			}
			CloseFile(fileHeader);
			CloseFile(fileCPP);
		}
#else
		//else
		{
			for (const auto& data : parser.GetAllFileParsedData())
			{
				if (std::filesystem::exists(data.FilePath + "/Generated"))
				{
					std::filesystem::remove_all(data.FilePath + "/Generated");
				}
			}

			for (const auto& data : parser.GetAllFileParsedData())
			{
				if (!std::filesystem::exists(data.FilePath + "/Generated"))
				{
					std::filesystem::create_directory(data.FilePath + "/Generated");
				}

				const std::string hFile = data.FilePath + "/Generated/" + data.FileName + ReflectFileGeneratePrefix + ".h";
				const std::string cppFile = data.FilePath + "/Generated/" + data.FileName + ReflectFileGeneratePrefix + ".cpp";

				std::ofstream file = OpenFile(hFile);
				header.GenerateHeader(data, file, addtionalOptions);
				CloseFile(file);

				file = OpenFile(cppFile);
				source.GenerateSource(data, file, addtionalOptions);
				CloseFile(file);
			}
		}
#endif
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
}
