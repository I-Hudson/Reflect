#pragma once

#include "Core/Core.h"
#include "ReflectStructs.h"
#include <FileParser/FileParser.h>
#include<fstream>

namespace Reflect
{
	/// <summary>
	/// Generate a new file and place the reflect code there.
	/// </summary>
	class CodeGenerate
	{
	public:
		REFLECT_API CodeGenerate();
		REFLECT_API ~CodeGenerate();

		REFLECT_API void Reflect(const FileParser& parser, const ReflectAddtionalOptions& addtionalOptions);
		REFLECT_API static void IncludeHeader(const std::string& headerToInclude, std::ofstream& file, bool windowsInclude = false);

	private:
		std::ofstream OpenFile(const std::string& filePath);
		void CloseFile(std::ofstream& file);
	};

#define WRITE_PUBLIC() file << "public:\\\n"
#define WRITE_PRIVATE() file << "private:\\\n"
#define TAB() '\t'
#define NEW_LINE() " \n"
#define NEW_LINE_SLASH() " \\\n"
#define TAB_NEW_LINE() "\t\n"
}