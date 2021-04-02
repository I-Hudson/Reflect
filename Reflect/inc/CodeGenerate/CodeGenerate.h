#pragma once

#include "Core/Core.h"
#include "ReflectStructs.h"
#include<fstream>

namespace Reflect
{
	/// <summary>
	/// Generate a new file and place the reflect code there.
	/// </summary>
	class CodeGenerate
	{
	public:
		REFLECT_DLL CodeGenerate();
		REFLECT_DLL ~CodeGenerate();

		REFLECT_DLL void Reflect(const FileParsedData& data);
		static void IncludeHeader(const std::string& headerToInclude, std::ofstream& file, bool windowsInclude = false);

	private:
		std::ofstream OpenFile(const std::string& filePath);
		void CloseFile(std::ofstream& file);
	};
}