#include "CodeGenerate/CPP/CG_CPP_TypeInfo.h"
#include "CodeGenerate/CodeGenerate.h"
#include "CodeGenerate/CG_Utils.h"

#include "Core/Core.h"

#include "Instrumentor.h"

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
		file << "template<>" << NEW_LINE;
		file << "class Reflect::GenerateTypeInfoForType<" + GetTypeName(data) + ">\n{" << NEW_LINE;
		file << "public:" << NEW_LINE;
		file << "\tReflect::ReflectTypeInfo GetTypeInfo(" + GetTypeName(data) + "* ownerClass)\n\t{" << NEW_LINE;
		file << "\t\tstd::vector<std::unique_ptr<Reflect::ReflectTypeInfo>> inheritances = GenerateInheritance(ownerClass);" << NEW_LINE;
		file << "\t\tstd::vector<std::unique_ptr<Reflect::ReflectTypeFunction>> functions = GenerateFunctions(ownerClass);" << NEW_LINE;
		file << "\t\tstd::vector<std::unique_ptr<Reflect::ReflectTypeMember>> members = GenerateMembers(ownerClass);" << NEW_LINE;
		file << "\t\tReflectTypeInfo reflect_type_info = ReflectTypeInfo(ownerClass," << NEW_LINE;
		file << "\t\t\tstd::make_unique<" << CG_Utils::WriteReflectTypeCPPDeclare(GetTypeName(data)) << ">";
		file << CG_Utils::WriteReflectTypeCPPParentheses(data.ReflectType, data.ReflectValueType, data.TypeInheritance, data.Name) << ", " << NEW_LINE;
		file << "\t\t\tstd::move(inheritances), " << NEW_LINE;
		file << "\t\t\tstd::move(members), " << NEW_LINE;
		file << "\t\t\tstd::move(functions));" << NEW_LINE;
		file << "\t\treflect_type_info.m_construct_func = [](){ return static_cast<void*>(new " + GetTypeName(data) + "); }; " << NEW_LINE;
		file << "\t\treturn reflect_type_info; " << NEW_LINE;
		file << "\t}" << NEW_LINE;
		file << "private:\n";

		WriteGenerateTypeInheritance(data, file, additionalOptions);
		WriteGenerateTypeMembers(data, file, additionalOptions);
		WriteGenerateTypeFunctions(data, file, additionalOptions);

		file << "};\n" << NEW_LINE;

		file << "Reflect::ReflectTypeInfo " + GetTypeName(data)  + "::GetStaticTypeInfo()\n{" << NEW_LINE;
		file << "\treturn Reflect::GenerateTypeInfoForType<" + GetTypeName(data) + ">().GetTypeInfo(nullptr);" << NEW_LINE;
		file << "}\n" << NEW_LINE;
		file << "Reflect::ReflectTypeInfo " + GetTypeName(data) + "::GetStaticTypeInfo(" + GetTypeName(data) + "* ownerPointer)\n{" << NEW_LINE;
		file << "\treturn Reflect::GenerateTypeInfoForType<" + GetTypeName(data) + ">().GetTypeInfo(ownerPointer);" << NEW_LINE;
		file << "}\n" << NEW_LINE;
		file << "Reflect::ReflectTypeInfo " + GetTypeName(data)  + "::GetTypeInfo()\n{" << NEW_LINE;
		file << "\treturn Reflect::GenerateTypeInfoForType<" + GetTypeName(data) + ">().GetTypeInfo(this);" << NEW_LINE;
		file << "}\n" << NEW_LINE;
	}

	void CG_CPP_TypeInfo::WriteGenerateTypeInheritance(const Parser::ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		file << "\tstd::vector<std::unique_ptr<Reflect::ReflectTypeInfo>> GenerateInheritance(" + GetTypeName(data) + "* ownerClass)\n\t{" << NEW_LINE;
		file << "\t\tstd::vector<std::unique_ptr<Reflect::ReflectTypeInfo>> inheritances;\n" << NEW_LINE;

		for (const auto& item : data.Inheritance)
		{
			if (!item.IsReflected) 
			{
				continue;
			}
			file << TAB << TAB << item.NameWithNamespace + "* " + item.Name + "_pointer = dynamic_cast<" + item.NameWithNamespace + "*>(ownerClass);" << NEW_LINE;
			//file << TAB << TAB << "assert(" + item.Name + "_pointer);" << NEW_LINE;
			file << TAB << TAB << "if(" + item.Name + "_pointer != nullptr)" << NEW_LINE;
			file << TAB << TAB << "{" << NEW_LINE;
			file << TAB << TAB << TAB << "inheritances.push_back(std::make_unique<Reflect::ReflectTypeInfo>(" + item.NameWithNamespace + "::GetStaticTypeInfo(" + item.Name + "_pointer)));" << NEW_LINE;
			file << TAB << TAB << "}" << NEW_LINE;
		}

		file << NEW_LINE;
		file << "\t\treturn inheritances;" << NEW_LINE;
		file << "\t}" << NEW_LINE;
	}

	void CG_CPP_TypeInfo::WriteGenerateTypeMembers(const Parser::ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		file << "\tstd::vector<std::unique_ptr<Reflect::ReflectTypeMember>> GenerateMembers(" + GetTypeName(data)  + "* ownerClass)\n\t{" << NEW_LINE;
		file << "\t\tstd::vector<std::unique_ptr<Reflect::ReflectTypeMember>> members;\n" << NEW_LINE;
		file << "\t\tstd::vector<std::string> flags;\n" << NEW_LINE;
		for (const auto& member : data.Members)
		{
			file << TAB << TAB << "flags.clear();" << NEW_LINE;
			int flagIndex = 0;
			for (const auto& flag : member.ContainerProps)
			{
				file << TAB << TAB << "flags.push_back(\"" + flag + "\");" << NEW_LINE;
			}

			file << "\t\tmembers.emplace_back(";
			file << "std::make_unique<Reflect::ReflectTypeMember>(";
			file << "ownerClass, ";
			file << "\"" + member.Name + "\"" + ", ";
			file << "(unsigned char*)ownerClass + offsetof(" + GetTypeName(data)  + ", " + member.Name + "), ";
			file << "std::make_unique<" << CG_Utils::WriteReflectTypeCPPDeclare(member.RawType) << ">";
			file << CG_Utils::WriteReflectTypeCPPParentheses(EReflectType::Member, member.ReflectValueType, member.TypeInheritance, member.Name) << ", ";
			file << "flags";
			file << ")); " << NEW_LINE << NEW_LINE;
		}
		file << "\t\treturn members;" << NEW_LINE;
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
				str += std::string("std::make_unique<") += CG_Utils::WriteReflectTypeCPPDeclare(arg.RawType) += ">";
				str += CG_Utils::WriteReflectTypeCPPParentheses(EReflectType::Parameter, arg.ReflectValueType, arg.TypeInheritance, arg.Name);
				return str;
			};

			std::string str;

			for (const auto& func : data.Functions)
			{
				str += "\t\tstd::vector<std::unique_ptr<ReflectType>> " + func.Name + "_Args;" + NEW_LINE;
				for (const auto& arg : func.Parameters)
				{
					str += "\t\t" + func.Name + "_Args.emplace_back(" + generateSingleArg(arg) + ");" + NEW_LINE;
				}
			}
			return str;
		};

		auto generateSingleFunction = [&data, this](const Parser::ReflectFunctionData& func) -> std::string
		{
			std::string str;
			str += "std::make_unique<Reflect::ReflectTypeFunction>";
			str += "((void*)ownerClass, ";
			str += "\"" + func.Name + "\"" + ", ";
			str += GetTypeName(data) + "::__REFLECT_FUNC__" + func.Name + ", ";
			str += std::string("std::make_unique<") += CG_Utils::WriteReflectTypeCPPDeclare(func.Type) += ">";
			str += CG_Utils::WriteReflectTypeCPPParentheses(EReflectType::Function, func.ReflectValueType, func.TypeInheritance, func.Name) += ", ";
			str += "std::move(" + func.Name + "_Args))";
			return str;
		};

		file << "\tstd::vector<std::unique_ptr<Reflect::ReflectTypeFunction>> GenerateFunctions(" + GetTypeName(data)  + "* ownerClass)\n\t{" << NEW_LINE;

		file << generateFunctionArgs() + NEW_LINE;

		file << "\t\tstd::vector<std::unique_ptr<Reflect::ReflectTypeFunction>> funcs;" << NEW_LINE;
		for (const auto& f : data.Functions)
		{
			file << "\t\tfuncs.push_back(" + generateSingleFunction(f) + ");" << NEW_LINE;
		}
		file << "\t\treturn funcs;" << NEW_LINE;
		file << "\t}" << NEW_LINE;
	}

	std::string CG_CPP_TypeInfo::GetTypeName(const Parser::ReflectContainerData& data) const
	{
		return data.NameWithNamespace;
	}
#endif
}