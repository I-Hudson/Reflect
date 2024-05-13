#pragma once

#include "Reflect/FileParser/ParserStructs.h"

namespace Reflect
{
    struct ReflectAddtionalOptions;
}

namespace Reflect::CodeGeneration
{
#ifdef REFLECT_TYPE_INFO_ENABLED
    class CG_CPP_TypeInfo
    {
    public:
        
        void Generate(const Parser::ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions);

    private:
        void WriteGenerateTypeInfo(const Parser::ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions);
        void WriteGenerateTypeInheritance(const Parser::ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions);
        void WriteGenerateTypeMembers(const Parser::ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions);
        void WriteGenerateTypeFunctions(const Parser::ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions);

        void WriteReflectCPPInheritanceChain(std::ofstream& file, EReflectType refectType, const Parser::ReflectTypeNameData& typeNameData, int lineIndent);

        std::string GetTypeName(const Parser::ReflectContainerData& data) const;
    };
#endif
}