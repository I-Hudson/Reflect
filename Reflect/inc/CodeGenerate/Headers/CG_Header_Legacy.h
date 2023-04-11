#pragma once

#include "Core/Defines.h"
#include "ReflectStructs.h"

namespace Reflect
{
	struct ReflectAddtionalOptions;

    class CG_Header_Legacy
    {
    public:
		static std::string GetType(const Reflect::ReflectTypeNameData& arg, bool defaultReturnPointer);

		void WriteGeneratedData(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const ReflectAddtionalOptions* additionalOptions);
        void WriteClosingMacro(std::ofstream& file, std::string_view currentFileId);

		static void WriteFunctions(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const ReflectAddtionalOptions* additionalOptions);
	private:

		void WriteMemberProperties(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const ReflectAddtionalOptions* additionalOptions);
		void WriteMemberPropertiesOffsets(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const ReflectAddtionalOptions* additionalOptions);
		void WriteMemberGet(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const ReflectAddtionalOptions* additionalOptions);

		void WriteFunctionGet(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const ReflectAddtionalOptions* additionalOptions);
    };
}
