#pragma once

#include "Core/Defines.h"
#include "ReflectStructs.h"

namespace Reflect
{
    struct ReflectAddtionalOptions;

    class CG_CPP_Legacy
    {
    public:

        void Generate(const ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions);

    private:
        void WriteMemberProperties(const ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions);

        void WriteMemberGet(const ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions);
        void WriteFunctionGet(const ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions);

        std::string MemberFormat();
    };
}