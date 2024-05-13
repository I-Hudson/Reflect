#pragma once

#include "Reflect/Core/Defines.h"
#include "Reflect/FileParser/FileParser.h"

#include<fstream>

namespace Reflect
{
	struct ReflectAddtionalOptions;
}

namespace Reflect::CodeGeneration
{
	/// <summary>
	/// Generate a new file and place the reflect code there.
	/// </summary>
	class CodeGenerate
	{
	public:
		REFLECT_API CodeGenerate();
		REFLECT_API ~CodeGenerate();

		REFLECT_API void Reflect(const Parser::FileParser& parser, const ReflectAddtionalOptions* additionalOptions);
		REFLECT_API static void IncludeHeader(const std::string& headerToInclude, std::ofstream& file, bool windowsInclude = false);

	private:
		std::ofstream OpenFile(const std::string& filePath);
		void CloseFile(std::ofstream& file);
	};

#define WRITE_CLOSE file << "\n\n"

#define WRITE_PUBLIC file << "public:\\\n"
#define WRITE_PRIVATE file << "private:\\\n"
#define TAB '\t'
#define TAB_N(x) for(int i = 0; i < x; ++i) { file << TAB; }
#define NEW_LINE " \n"
#define NEW_LINE_SLASH " \\\n"
#define TAB_NEW_LINE " \t\n"
}