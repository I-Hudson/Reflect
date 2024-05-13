#pragma once

#include "Reflect/Core/Defines.h"
#include "Reflect/FileParser/ParserStructs.h"

namespace Reflect
{
	struct ReflectAddtionalOptions;
}

namespace Reflect::CodeGeneration
{
    class CG_Header_Legacy
    {
    public:
		static std::string GetType(const Parser::ReflectTypeNameData& arg, bool defaultReturnPointer);

		void WriteGeneratedData(const Parser::ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const ReflectAddtionalOptions* additionalOptions);
        void WriteClosingMacro(std::ofstream& file, std::string_view currentFileId);

		static void WriteFunctions(const Parser::ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const ReflectAddtionalOptions* additionalOptions);
	private:

		void WriteMemberProperties(const Parser::ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const ReflectAddtionalOptions* additionalOptions);
		void WriteMemberPropertiesOffsets(const Parser::ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const ReflectAddtionalOptions* additionalOptions);
		void WriteMemberGet(const Parser::ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const ReflectAddtionalOptions* additionalOptions);

		void WriteFunctionGet(const Parser::ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const ReflectAddtionalOptions* additionalOptions);
    };
}
