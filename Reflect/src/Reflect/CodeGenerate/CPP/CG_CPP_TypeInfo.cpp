#include "Reflect/CodeGenerate/CPP/CG_CPP_TypeInfo.h"
#include "Reflect/CodeGenerate/CodeGenerate.h"
#include "Reflect/CodeGenerate/CG_Utils.h"

#include "Reflect/Core/Core.h"

#include "Reflect/Instrumentor.h"

#include <unordered_set>
#include <functional>

namespace Reflect::CodeGeneration
{
#ifdef REFLECT_TYPE_INFO_ENABLED

    void CG_CPP_TypeInfo::Generate(const Parser::ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions)
    {
		REFLECT_PROFILE_FUNCTION();
        WriteGenerateTypeInfo(data, file, additionalOptions);
    }

	void CG_CPP_TypeInfo::WriteGenerateTypeInfo(const Parser::ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();

		file << "Reflect::TypeInfoRegister " << GetTypeName(data) << "::s_TypeInfoRegister";
		file << " = Reflect::TypeInfoRegister(\"" << data.NameWithNamespace << "\", " << data.NameWithNamespace << "::ReflectRegisterCallback); " << NEW_LINE;
		file << NEW_LINE;

		file << "namespace Reflect" << NEW_LINE;
		file << "{" << NEW_LINE;
		file << NEW_LINE;

		file << "static TypeInfo s_" << data.Name << "TypeInfo;";
		file << NEW_LINE;
		file << "static bool s_" << data.Name << "Initialised = false;";
		file << NEW_LINE;
		file << NEW_LINE;

		file << "template<>" << NEW_LINE;
		file << "class GenerateTypeInfoForType<" << GetTypeName(data) << ">\n{" << NEW_LINE;
		file << "public:" << NEW_LINE;
		file << "\tTypeInfo GetTypeInfo(" << GetTypeName(data) << "* objectInstance)\n\t{" << NEW_LINE;

		TAB_N(2); file << "if (!s_" << data.Name << "Initialised)" << NEW_LINE;
		TAB_N(2); file << "{" << NEW_LINE;

		TAB_N(3); file << "std::vector<TypeInfo> parentTypeInfos = GenerateParentTypeInfos(objectInstance);" << NEW_LINE;
		TAB_N(3); file << "std::vector<FunctionInfo> functionInfos = GenerateFunctionInfos(objectInstance);" << NEW_LINE;
		TAB_N(3); file << "std::vector<MemberInfo> memberInfos = GenerateMemberInfos(objectInstance);" << NEW_LINE;
		TAB_N(3); file << "std::vector<Type> " << data.Name << "_InheritanceTypes;" << NEW_LINE;
		TAB_N(3); file << NEW_LINE;
				  
		TAB_N(3); file << "s_" << data.Name << "TypeInfo = TypeInfo(" << NEW_LINE;
		TAB_N(4); file << "Type::MakeType<::" << data.NameWithNamespace << ">(), " << NEW_LINE;
		TAB_N(4); file << "objectInstance, " << NEW_LINE;
		TAB_N(4); file << "std::move(parentTypeInfos), " << NEW_LINE;
		TAB_N(4); file << "std::move(memberInfos), " << NEW_LINE;
		TAB_N(4); file << "std::move(functionInfos));" << NEW_LINE;

		TAB_N(3); file << "s_" << data.Name << "Initialised = true;" << NEW_LINE;
		TAB_N(2); file << "}" << NEW_LINE;
		file << NEW_LINE;

		TAB_N(2);  file << "TypeInfo typeInfo = s_" << data.Name << "TypeInfo;" << NEW_LINE;
		TAB_N(2);  file << "typeInfo.SetObjectInstance(objectInstance ? objectInstance : nullptr);" << NEW_LINE;
		file << "\t\treturn typeInfo;" << NEW_LINE;
		file << "\t};" << NEW_LINE;

		file << "private:\n";

		WriteGenerateTypeInheritance(data, file, additionalOptions);
		WriteGenerateTypeMembers(data, file, additionalOptions);
		WriteGenerateTypeFunctions(data, file, additionalOptions);

		file << "};" << NEW_LINE <<  NEW_LINE;

		file << "}" << NEW_LINE;

		file << "Reflect::TypeInfo " + GetTypeName(data)  + "::GetStaticTypeInfo()\n{" << NEW_LINE;
		file << "\treturn Reflect::GenerateTypeInfoForType<" + GetTypeName(data) + ">().GetTypeInfo(nullptr);" << NEW_LINE;
		file << "}\n" << NEW_LINE;

		file << "Reflect::TypeInfo " + GetTypeName(data) + "::GetStaticTypeInfo(" + GetTypeName(data) + "* ownerPointer)\n{" << NEW_LINE;
		file << "\treturn Reflect::GenerateTypeInfoForType<" + GetTypeName(data) + ">().GetTypeInfo(ownerPointer);" << NEW_LINE;
		file << "}\n" << NEW_LINE;

		file << "Reflect::TypeInfo " + GetTypeName(data)  + "::GetTypeInfo()\n{" << NEW_LINE;
		file << "\treturn Reflect::GenerateTypeInfoForType<" + GetTypeName(data) + ">().GetTypeInfo(this);" << NEW_LINE;
		file << "}\n" << NEW_LINE;

		file << "Reflect::TypeInfo " + GetTypeName(data) + "::ReflectRegisterCallback(void* objectInstance)\n{" << NEW_LINE;
		file << "\treturn " + GetTypeName(data) + "::GetStaticTypeInfo(static_cast<" + GetTypeName(data) + "*>(objectInstance)); " << NEW_LINE;
		file << "}\n" << NEW_LINE;
	}

	void CG_CPP_TypeInfo::WriteGenerateTypeInheritance(const Parser::ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		file << "\tstd::vector<TypeInfo> GenerateParentTypeInfos(" + GetTypeName(data) + "* objectInstance)\n\t{" << NEW_LINE;
		file << "\t\tstd::vector<TypeInfo> parentTypeInfos;\n" << NEW_LINE;

		for (const auto& item : data.Inheritance)
		{
			if (!item.IsReflected) 
			{
				continue;
			}
			file << TAB << TAB << item.NameWithNamespace + "* " + item.Name + "_pointer = dynamic_cast<" + item.NameWithNamespace + "*>(objectInstance);" << NEW_LINE;
			file << TAB << TAB << TAB << "parentTypeInfos.push_back(" + item.NameWithNamespace + "::GetStaticTypeInfo(" + item.Name + "_pointer));" << NEW_LINE;
			file << NEW_LINE;
		}

		file << "\t\treturn parentTypeInfos;" << NEW_LINE;
		file << "\t}" << NEW_LINE;
	}

	void CG_CPP_TypeInfo::WriteGenerateTypeFunctions(const Parser::ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		auto generateFunctionArgs = [&data]()->std::string
		{
			auto generateSingleArg = [&data](const Parser::ReflectTypeNameData& arg) -> std::string
			{
				
				std::string str;
				if (arg.NameWithNamespace.empty())
				{
					str += "Type::MakeType<" + arg.RawType + ">()";
				}
				else
				{
					str += "Type::MakeType<::" + arg.NameWithNamespace + ">()";
				}
				return str;
			};

			std::string str;

			for (const auto& func : data.Functions)
			{
				str += "\t\tstd::vector<Type> " + func.Name + "_Args;" + NEW_LINE;
				for (const auto& arg : func.Parameters)
				{
					str += "\t\t" + func.Name + "_Args.emplace_back(" + generateSingleArg(arg) + ");" + NEW_LINE;
				}
				str += NEW_LINE;
			}
			return str;
		};

		auto generateFunctionFlags = [&data]()->std::string
			{
				std::string str;
				for (const auto& func : data.Functions)
				{
					str += "\t\tstd::vector<std::string> " + func.Name + "_Flags;" + NEW_LINE;
					for (const auto& arg : func.ContainerProps)
					{
						str += "\t\t" + func.Name + "_Flags.emplace_back(\"" + arg + "\");" + NEW_LINE;
					}
					str += NEW_LINE;
				}
				return str;
			};

		auto generateFunctionPropertyMetas = [&data]()->std::string
			{
				std::string str;
				for (const auto& func : data.Functions)
				{
					str += "\t\tstd::vector<PropertyMeta> " + func.Name + "_PropertyMetas;" + NEW_LINE;
					for (const auto& propertyMeta : func.PropertyMetas)
					{
						str += "\t\t" + func.Name + "_PropertyMetas.emplace_back(PropertyMeta(\"" + propertyMeta.GetKey() + "\", \"" + propertyMeta.GetValue() + "\"));" + NEW_LINE;
					}
					str += NEW_LINE;
				}
				return str;
			};

		auto generateSingleFunction = [&data, this](const Parser::ReflectFunctionData& func) -> std::string
		{
			std::string str;
			str += "FunctionInfo";
			if (func.NameWithNamespace.empty())
			{
				str += "(Type::MakeType<" + func.Type + ">(), ";
			}
			else
			{
				str += "(Type::MakeType<::" + func.NameWithNamespace + ">(), ";
			}
			str += "\"" + func.Name + "\"" + ", ";
			str += "std::move(" + func.Name + "_Args), ";
			str += "std::move(" + func.Name + "_Flags), ";
			str += "std::move(" + func.Name + "_PropertyMetas), ";
			str += GetTypeName(data) + "::__REFLECT_FUNC__" + func.Name + ", ";
			str += "objectInstance";
			return str;
		};

		file << "\tstd::vector<FunctionInfo> GenerateFunctionInfos(" + GetTypeName(data)  + "* objectInstance)\n\t{" << NEW_LINE;

		file << generateFunctionArgs();
		file << generateFunctionFlags();
		file << generateFunctionPropertyMetas();

		file << "\t\tstd::vector<FunctionInfo> functionInfos;" << NEW_LINE;
		for (const auto& f : data.Functions)
		{
			file << "\t\tfunctionInfos.push_back(" + generateSingleFunction(f) + "));" << NEW_LINE;
		}
		file << "\t\treturn functionInfos;" << NEW_LINE;
		file << "\t}" << NEW_LINE;
	}

	void CG_CPP_TypeInfo::WriteGenerateTypeMembers(const Parser::ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		file << "\tstd::vector<MemberInfo> GenerateMemberInfos(" + GetTypeName(data)  + "* objectInstance)\n\t{" << NEW_LINE;
		file << "\t\tstd::vector<MemberInfo> memberInfos;\n" << NEW_LINE;
		file << "\t\tstd::vector<std::string> flags;\n" << NEW_LINE;
		file << "\t\tstd::vector<PropertyMeta> propertyMeta;\n" << NEW_LINE;

		for (const auto& member : data.Members)
		{
			int blockBracketIndent = 2;
			TAB_N(blockBracketIndent);
			file << "{\n";

			int lineIndent = 3;
#if 1
			//WriteReflectCPPInheritanceChain(file, EReflectType::Member, member, lineIndent);
#else
			file << "\t\tinheritanceTypes.clear();\n";
			std::vector<std::string> orderdInheritance = OrderMemberInheritance(member);
			//for (const Parser::ReflectInheritanceData& inheritanceData : member.TypeInheritance)
			for (const std::string& inheritance : orderdInheritance)
			{
				file << "\t\tstd::unique_ptr<" << CG_Utils::WriteReflectTypeCPPDeclare(inheritance) << ">";
				file << inheritance << "_Inheritance_" << member.Name << " = ";
				file << "std::make_unique<" << CG_Utils::WriteReflectTypeCPPDeclare(inheritance) << ">";
			}

			file << "\t\tstd::vector<std::unique_ptr<::Reflect::ReflectType>> " << member.Name << "_InheritanceTypes;\n";
#endif

			TAB_N(lineIndent);
			file << "flags.clear();" << NEW_LINE;
			TAB_N(lineIndent);
			file << "propertyMeta.clear();" << NEW_LINE;
			file << NEW_LINE;

			for (const auto& flag : member.ContainerProps)
			{
				TAB_N(lineIndent);
				file << "flags.push_back(\"" + flag + "\");" << NEW_LINE;
			}

			for (const auto& propertyMeta : member.PropertyMetas)
			{
				TAB_N(lineIndent);
				file << "propertyMeta.push_back(PropertyMeta(\"" << propertyMeta.GetKey() << "\", \"" << propertyMeta.GetValue() << "\"));" << NEW_LINE;
			}
			file << NEW_LINE;

			TAB_N(lineIndent);
			file << "memberInfos.emplace_back(\n";

			TAB_N(lineIndent + 1);
			file << "MemberInfo(" << NEW_LINE;

			TAB_N(lineIndent + 1);
			if (member.IsTemplate)
			{
				file << "Type::MakeType<::" + member.TemplateFirstType + "<::" + member.TemplateSecondType + ">>(), " << NEW_LINE;
			}
			else if (member.NameWithNamespace.empty())
			{
				file << "Type::MakeType<" + member.Type + ">(), " << NEW_LINE;
			}
			else
			{
				file << "Type::MakeType<::" + member.NameWithNamespace + ">(), " << NEW_LINE;
			}

			TAB_N(lineIndent + 1);
			file << "\"" + member.Name + "\"" + ", " << NEW_LINE;

			TAB_N(lineIndent + 1);
			file << "static_cast<EReflectValueType>(" + std::to_string(static_cast<int>(member.ReflectValueType)) + "), " << NEW_LINE;

			TAB_N(lineIndent + 1);
			file << "static_cast<EReflectValueModifier>(" + std::to_string(static_cast<int>(member.ReflectModifier)) + "), " << NEW_LINE;

			TAB_N(lineIndent + 1);
			file << "flags, " << NEW_LINE;

			TAB_N(lineIndent + 1);
			file << "propertyMeta, " << NEW_LINE;

			TAB_N(lineIndent + 1);
			file << "offsetof(::" + data.NameWithNamespace + ", " + member.Name + "), " << NEW_LINE;

			TAB_N(lineIndent + 1);
			file << "objectInstance" << NEW_LINE;
			
			TAB_N(lineIndent + 1);
			file << "));" << NEW_LINE;

			TAB_N(blockBracketIndent);
			file << "}\n\n";
		}
		file << "\t\treturn memberInfos;" << NEW_LINE;
		file << "\t}" << NEW_LINE;
	}

	void CG_CPP_TypeInfo::WriteReflectCPPInheritanceChain(std::ofstream& file, EReflectType reflectType, const Parser::ReflectTypeNameData& typeNameData, int indent)
	{
		std::function<void(const Parser::ReflectInheritanceData& data)> writeInheritance =
			[&](const Parser::ReflectInheritanceData& data)
			{
				if (!data.IsReflected)
				{
					return;
				}

				for (const Parser::ReflectInheritanceData& d : data.Inheritances)
				{
					writeInheritance(d);
				}

				if (!data.Inheritances.empty())
				{
					TAB_N(indent);
					file << "std::vector<std::unique_ptr<::Reflect::ReflectType>> " << typeNameData.Name << "_" << data.Name << "_InheritanceChain;\n";

					for (const Parser::ReflectInheritanceData& d : data.Inheritances)
					{
						if (!d.IsReflected)
						{
							continue;
						}

						TAB_N(indent + 1);
						file << typeNameData.Name << "_" << data.Name << "_InheritanceChain.push_back(std::move(";
						file << typeNameData.Name << "_" << d.Name << "));\n";
					}
				}

				TAB_N(indent);
				file << "std::unique_ptr<" << CG_Utils::WriteReflectTypeCPPDeclare(data.NameWithNamespace) << "> ";
				file << typeNameData.Name << "_" << data.Name << " = ";
				file << "std::make_unique<" << CG_Utils::WriteReflectTypeCPPDeclare(data.NameWithNamespace) << ">";
				file << CG_Utils::WriteReflectTypeCPPParentheses(reflectType, typeNameData.ReflectValueType, data.Inheritances, data.Name, typeNameData.Name) << ";";
				file << "\n";
			};

		TAB_N(indent);
		file << "std::vector<std::unique_ptr<::Reflect::ReflectType>> " << typeNameData.Name << "_InheritanceChain;\n";
		for (const Parser::ReflectInheritanceData& data : typeNameData.TypeInheritance)
		{
			writeInheritance(data);
			TAB_N(indent);
			file << typeNameData.Name << "_InheritanceChain.push_back(std::move(";
			file << typeNameData.Name << "_" << data.Name << "));\n";
		}

		file << "\n";		
	}

	std::string CG_CPP_TypeInfo::GetTypeName(const Parser::ReflectContainerData& data) const
	{
		return data.NameWithNamespace;
	}
#endif
}