#pragma once

#include "ReflectStructs.h"

namespace Reflect
{
#ifdef REFLECT_TYPE_INFO_ENABLED

    struct ReflectAddtionalOptions;

    class CG_CPP_TypeInfo
    {
    public:
        
        void Generate(const ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions);

    private:
        void WriteGenerateTypeInfo(const ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions);
        void WriteGenerateTypeInheritance(const ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions);
        void WriteGenerateTypeMembers(const ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions);
        void WriteGenerateTypeFunctions(const ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions);
    };
#endif
}