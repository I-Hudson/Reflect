#pragma once

#include "Core/Defines.h"
#include "FileParser/ParserStructs.h"
#include "ReflectStructs.h"

#include <string>
#include <vector>

namespace Reflect::CodeGeneration
{
    class CG_Utils
    {
    public:
        static std::string WriteReflectTypeCPPDeclare(std::string_view type);
        static std::string WriteReflectTypeCPPParentheses(EReflectType reflectType, EReflectValueType valueType, const std::vector<Parser::ReflectTypeNameData>& inheritance, std::string_view name);

        static std::string WriteReflectTypeCPPMember(Parser::ReflectMemberData memberData);
        static std::string WriteReflectTypeCPPFunction(Parser::ReflectFunctionData functionData);
        static std::string WriteReflectTypeCPPParameter(Parser::ReflectTypeNameData parameterData);

    private:
        static std::string WriteReflectTypeCPP(std::string_view type, EReflectType reflectType, EReflectValueType valueType, const std::vector<Parser::ReflectTypeNameData>& inheritance, std::string_view name);
    };
}