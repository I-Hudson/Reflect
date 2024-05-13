#include "Reflect/CodeGenerate/CodeGenerate.h"
#include "Reflect/CodeGenerate/CodeGenerateHeader.h"
#include "Reflect/CodeGenerate/CodeGenerateSource.h"

#include "Reflect/Core/Core.h"
#include "Reflect/Instrumentor.h"

#include <assert.h>
#include <filesystem>

namespace Reflect::CodeGeneration
{
	constexpr const char* ContainerPrefix = "ReflectObject";

	CodeGenerate::CodeGenerate()
	{ }

	CodeGenerate::~CodeGenerate()
	{ }

	void CodeGenerate::Reflect(const Parser::FileParser& parser, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();

		CodeGenerateHeader header;
		CodeGenerateSource source;

#ifdef REFLECT_SINGLE_FILE
		//if (additionalOptions->GetOption(Reflect::Reflect_CMD_Option_Single_File_EXT) == "true")
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
				if (data.parserOptions.DoNotReflect
				|| data.ReflectData.empty())
				{
					continue;
				}
				header.GenerateHeader(data, fileHeader, additionalOptions);
				source.GenerateSource(data, fileCPP, additionalOptions);
			}
			CloseFile(fileHeader);
			CloseFile(fileCPP);
		}
#else
		//else
		{
			{
				REFLECT_PROFILE_SCOPE("Remove all old gen files");
				for (const auto& data : parser.GetAllFileParsedData())
				{
					if (data.parserOptions.DoNotReflect)
					{
						continue;
					}

					if (std::filesystem::exists(data.FilePath + "/Generated"))
					{
						std::filesystem::remove_all(data.FilePath + "/Generated");
					}
				}
			}

			for (const auto& data : parser.GetAllFileParsedData())
			{
				if (data.parserOptions.DoNotReflect
					|| data.ReflectData.empty())
				{
					continue;
				}

				{
					REFLECT_PROFILE_SCOPE("Create gen folders");
					if (!std::filesystem::exists(data.FilePath + "/Generated"))
					{
						std::filesystem::create_directory(data.FilePath + "/Generated");
					}
				}

				const std::string hFile = data.FilePath + "/Generated/" + data.FileName + Keys::ReflectFileGeneratePrefix + ".h";
				const std::string cppFile = data.FilePath + "/Generated/" + data.FileName + Keys::ReflectFileGeneratePrefix + ".cpp";

				std::ofstream file = OpenFile(hFile);
				header.GenerateHeader(data, file, additionalOptions);
				CloseFile(file);

				file = OpenFile(cppFile);
				source.GenerateSource(data, file, additionalOptions);
				CloseFile(file);
			}
		}
#endif
	}

	std::ofstream CodeGenerate::OpenFile(const std::string& filePath)
	{
		REFLECT_PROFILE_FUNCTION();
		std::ofstream file;
		file.open(filePath, std::ios::trunc);
		assert(file.is_open() && "[CodeGenerate::OpenFile] File could not be created.");
		return file;
	}

	void CodeGenerate::CloseFile(std::ofstream& file)
	{
		REFLECT_PROFILE_FUNCTION();
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
