#pragma once

#include "Core/Defines.h"
#include "ReflectStructs.h"

namespace Reflect
{
#ifdef REFLECT_TYPE_INFO_ENABLED

    struct ReflectAddtionalOptions;

    class CG_Header_TypeInfo
    {
    public:
        void WriteGenerateTypeInfo(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const ReflectAddtionalOptions* additionalOptions);
        void WriteClosingMacro(std::ofstream& file, std::string_view currentFileId);
    };
#endif
}
