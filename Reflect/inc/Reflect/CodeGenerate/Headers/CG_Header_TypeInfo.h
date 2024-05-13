#pragma once

#include "Reflect/Core/Defines.h"
#include "Reflect/FileParser/ParserStructs.h"

namespace Reflect
{
    struct ReflectAddtionalOptions;
}

namespace Reflect::CodeGeneration
{
#ifdef REFLECT_TYPE_INFO_ENABLED
    class CG_Header_TypeInfo
    {
    public:
        void WriteGenerateTypeInfo(const Parser::ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const ReflectAddtionalOptions* additionalOptions);
        void WriteClosingMacro(std::ofstream& file, std::string_view currentFileId);
    };
#endif
}
