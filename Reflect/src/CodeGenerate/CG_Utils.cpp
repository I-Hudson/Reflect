#include "CodeGenerate/CG_Utils.h"
#include <fstream>

namespace Reflect::CodeGeneration
{
    std::string CG_Utils::WriteReflectTypeCPPDeclare(std::string_view type)
    {
        std::string str;
        str += "::Reflect::ReflectTypeCPP<"; str += type; str += ">";
        return str;
    }

    std::string CG_Utils::WriteReflectTypeCPPParentheses(EReflectType reflectType, EReflectValueType valueType, const std::vector<Parser::ReflectTypeNameData>& inheritance, std::string_view name)
    {
        std::string str;
        str += "(";
        str += "static_cast<::Reflect::EReflectType>(" + std::to_string(static_cast<int>(reflectType)) + "), ";
        str += "static_cast<::Reflect::EReflectValueType>(" + std::to_string(static_cast<int>(valueType)) + "), ";
        str += "std::move(std::vector<std::unique_ptr<::Reflect::ReflectType>>\n";
        //if (!inheritance.empty())
        {
            str += "\t{";
            for (const Parser::ReflectTypeNameData& type : inheritance)
            {
                str += "\t\tstd::make_unique<" + WriteReflectTypeCPPDeclare(type.Type) + ">" + WriteReflectTypeCPPParentheses(reflectType, type.ReflectValueType, type.TypeInheritance, type.Name) + ", \n";
            }
            str += "\t}";
        }
        str += "), \n";
        str += "\""; str += name; str += "\"";
        str += ")";
        return str;
    }

    std::string CG_Utils::WriteReflectTypeCPPMember(Parser::ReflectMemberData memberData)
    {
        return WriteReflectTypeCPP(memberData.Type, EReflectType::Member, memberData.ReflectValueType, memberData.TypeInheritance, memberData.Name);
    }

    std::string CG_Utils::WriteReflectTypeCPPFunction(Parser::ReflectFunctionData functionData)
    {
        return WriteReflectTypeCPP(functionData.Type, EReflectType::Function, functionData.ReflectValueType, functionData.TypeInheritance, functionData.Name);
    }

    std::string CG_Utils::WriteReflectTypeCPPParameter(Parser::ReflectTypeNameData parameterData)
    {
        return WriteReflectTypeCPP(parameterData.Type, EReflectType::Parameter, parameterData.ReflectValueType, parameterData.TypeInheritance, parameterData.Name);
    }

    std::string CG_Utils::WriteReflectTypeCPP(std::string_view type, EReflectType reflectType, EReflectValueType valueType, const std::vector<Parser::ReflectTypeNameData>& inheritance, std::string_view name)
    {
        std::string str;
        str += WriteReflectTypeCPPDeclare(type);
        str += WriteReflectTypeCPPParentheses(reflectType, valueType, inheritance, name);
        return str;
    }
}