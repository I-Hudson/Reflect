#include "CodeGenerate/CG_Utils.h"
#include <fstream>

namespace Reflect::CodeGeneration
{
    std::string CG_Utils::WriteReflectTypeCPPDeclare(std::string_view type)
    {
        std::string str;
        str += "ReflectTypeCPP<"; str += type; str += ">";
        return str;
    }

    std::string CG_Utils::WriteReflectTypeCPPParentheses(EReflectType reflectType, EReflectValueType valueType, std::string_view name)
    {
        std::string str;
        str += "(";
        str += "static_cast<Reflect::EReflectType>(" + std::to_string(static_cast<int>(reflectType)) + "), ";
        str += "static_cast<EReflectValueType>(" + std::to_string(static_cast<int>(valueType)) + "), ";
        str += "std::vector<ReflectType*>(), ";
        str += "\""; str += name; str += "\"";
        str += ")";
        return str;
    }

    std::string CG_Utils::WriteReflectTypeCPPMember(Parser::ReflectMemberData memberData)
    {
        return WriteReflectTypeCPP(memberData.Type, EReflectType::Member, memberData.ReflectValueType, memberData.Name);
    }

    std::string CG_Utils::WriteReflectTypeCPPFunction(Parser::ReflectFunctionData functionData)
    {
        return WriteReflectTypeCPP(functionData.Type, EReflectType::Function, functionData.ReflectValueType, functionData.Name);
    }

    std::string CG_Utils::WriteReflectTypeCPPParameter(Parser::ReflectTypeNameData parameterData)
    {
        return WriteReflectTypeCPP(parameterData.Type, EReflectType::Parameter, parameterData.ReflectValueType, parameterData.Name);
    }

    std::string CG_Utils::WriteReflectTypeCPP(std::string_view type, EReflectType reflectType, EReflectValueType valueType, std::string_view name)
    {
        std::string str;
        str += WriteReflectTypeCPPDeclare(type);
        str += WriteReflectTypeCPPParentheses(reflectType, valueType, name);
        return str;
    }
}