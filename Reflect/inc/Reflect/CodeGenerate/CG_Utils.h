#pragma once

#include "Reflect/Core/Defines.h"
#include "Reflect/FileParser/ParserStructs.h"
#include "Reflect/ReflectStructs.h"

#include <string>
#include <vector>

namespace Reflect::CodeGeneration
{
    class CG_Utils
    {
    public:
        static std::string WriteReflectTypeCPPDeclare(std::string_view type);
        static std::string WriteReflectTypeCPPParentheses(EReflectType reflectType, EReflectValueType valueType, const std::vector<Parser::ReflectInheritanceData>& inheritance, std::string_view name, std::string_view instanceChaiPrefix = "");

        static std::string WriteReflectTypeCPPMember(Parser::ReflectMemberData memberData);
        static std::string WriteReflectTypeCPPFunction(Parser::ReflectFunctionData functionData);
        static std::string WriteReflectTypeCPPParameter(Parser::ReflectTypeNameData parameterData);

        static void WriteIfDefines(const Parser::ReflectContainerData& data, std::ofstream& file);
        static void WriteEndIfDefines(const Parser::ReflectContainerData& data, std::ofstream& file);

    private:
        static std::string WriteReflectTypeCPP(std::string_view type, EReflectType reflectType, EReflectValueType valueType, const std::vector<Parser::ReflectInheritanceData>& inheritance, std::string_view name);
    };
}