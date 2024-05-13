#include "Reflect/CodeGenerate/CPP/CG_CPP_Legacy.h"
#include "Reflect/CodeGenerate/Headers/CG_Header_Legacy.h"
#include "Reflect/CodeGenerate/CodeGenerate.h"
#include "Reflect/CodeGenerate/CG_Utils.h"

#include "Reflect/Core/Core.h"
#include "Reflect/Instrumentor.h"

#include <functional>

namespace Reflect::CodeGeneration
{
	void CG_CPP_Legacy::Generate(const Parser::ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		WriteMemberProperties(data, file, additionalOptions);
		WriteFunctionGet(data, file, additionalOptions);
		WriteMemberGet(data, file, additionalOptions);
	}

	void CG_CPP_Legacy::WriteMemberProperties(const Parser::ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		auto getMemberProps = [](const std::vector<std::string>& flags) -> std::string
		{
			if (flags.size() == 0)
			{
				return "{ }";
			}

			std::string value;
			value += "{";
			for (auto const& flag : flags)
			{
				if (flag != flags.back())
				{
					value += "\"" + flag + "\"" + ", ";
				}
			}
			value += "\"" + flags.back() + "\"" + "}";
			return value;
		};

		if (data.Members.size() > 0)
		{
			file << "Reflect::ReflectMemberProp " + data.NameWithNamespace + "::__REFLECT_MEMBER_PROPS__[" + std::to_string(data.Members.size()) + "] = {" << NEW_LINE;
			for (const auto& member : data.Members)
			{
				file << "\tReflect::ReflectMemberProp(\"" + member.Name + "\", ";
				file << "new " << CG_Utils::WriteReflectTypeCPPDeclare(member.RawType);
				file << CG_Utils::WriteReflectTypeCPPParentheses(EReflectType::Member, member.ReflectValueType, {}, member.Name) << ", ";
				file << "__REFLECT__" + member.Name + "(), ";
				file << getMemberProps(member.ContainerProps) + ")," + NEW_LINE;
			}
			file << "};\n" << NEW_LINE;

			file << "void " + data.NameWithNamespace + "::InitialiseMembers()" << NEW_LINE;
			file << "{\n";
			for (const auto& inheritanceData : data.Inheritance)
			{
				if (inheritanceData.Name != "REFLECT_BASE"
					&& inheritanceData.Name != "REFLECT_BASE()")
				{
					TAB_N(1);
					file << inheritanceData.NameWithNamespace << "::InitialiseMembers();\n";
				}
			}
			for (const auto& member : data.Members)
			{
				WriteReflectCPPInheritanceChain(file, EReflectType::Member, member, 1);
			}
			file << "}\n";
		}
	}

	void CG_CPP_Legacy::WriteMemberGet(const Parser::ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		file << "Reflect::ReflectMember " + data.NameWithNamespace + "::GetMember(const char* memberName)\n{" << NEW_LINE;
		if (data.Members.size() > 0)
		{
			TAB_N(1);
			file << "for(const auto& member : __REFLECT_MEMBER_PROPS__)\n\t{" << NEW_LINE;
			TAB_N(2);
			file << "if(memberName == member.Name)" << NEW_LINE;
			TAB_N(2);
			file << "{" << NEW_LINE;
			TAB_N(3);
			file << "//CheckFlags" << NEW_LINE;
			TAB_N(3);
			file << "return " + MemberFormat() + "; " << NEW_LINE;
			TAB_N(2);
			file << "}" << NEW_LINE;
			TAB_N(1);
			file << "}" << NEW_LINE;
		}
		file << "\treturn __super::GetMember(memberName);" << NEW_LINE;
		file << "}\n" << NEW_LINE;

		file << "std::vector<Reflect::ReflectMember> " + data.NameWithNamespace + "::GetMembers(std::vector<std::string> const& flags)\n{" << NEW_LINE;
		file << "\tstd::vector<Reflect::ReflectMember> members = __super::GetMembers(flags);" << NEW_LINE;
		if (data.Members.size() > 0)
		{
			TAB_N(1);
			file << "for(auto& member : __REFLECT_MEMBER_PROPS__)\n\t{" << NEW_LINE;
			TAB_N(2);
			file << "if(member.ContainsProperty(flags))" << NEW_LINE;
			TAB_N(2);
			file << "{" << NEW_LINE;
			TAB_N(3);
			file << "members.push_back(" + MemberFormat() + "); " << NEW_LINE;
			TAB_N(2);
			file << "}" << NEW_LINE;
			TAB_N(1);
			file << "}" << NEW_LINE;
		}
		file << "\treturn members;" << NEW_LINE;
		file << "}\n" << NEW_LINE;

		file << "std::vector<Reflect::ReflectMember> " + data.NameWithNamespace + "::GetAllMembers()\n{" << NEW_LINE;
		file << "\tstd::vector<Reflect::ReflectMember> members = __super::GetAllMembers();" << NEW_LINE;
		if (data.Members.size() > 0)
		{
			TAB_N(1);
			file << "for(auto& member : __REFLECT_MEMBER_PROPS__)\n\t{" << NEW_LINE;
			TAB_N(2);
			file << "members.push_back(" + MemberFormat() + "); " << NEW_LINE;
			TAB_N(1);
			file << "}" << NEW_LINE;
		}
		file << "\treturn members;" << NEW_LINE;
		file << "}\n" << NEW_LINE;
	}

	void CG_CPP_Legacy::WriteFunctionGet(const Parser::ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		file << "Reflect::ReflectFunction " + data.NameWithNamespace + "::GetFunction(const char* functionName)\n{" << NEW_LINE;
		for (const auto& func : data.Functions)
		{
			TAB_N(1);
			file << "if(functionName == \"" + func.Name + "\")" << NEW_LINE;
			TAB_N(1);
			file << "{" << NEW_LINE;
			TAB_N(2);
			file << "return Reflect::ReflectFunction(this, " + data.NameWithNamespace + "::__REFLECT_FUNC__" + func.Name + ");" << NEW_LINE;
			TAB_N(1);
			file << "}" << NEW_LINE;
		}
		file << "\treturn __super::GetFunction(functionName);" << NEW_LINE;
		file << "}\n" << NEW_LINE;
	}

	void CG_CPP_Legacy::WriteReflectCPPInheritanceChain(std::ofstream& file, EReflectType reflectType, const Parser::ReflectTypeNameData& typeNameData, int indent)
	{
		std::function<void(const Parser::ReflectInheritanceData& data)> writeInheritance =
			[&](const Parser::ReflectInheritanceData& data)
			{
				TAB_N(indent + 1);
				file << "std::unique_ptr<" << CG_Utils::WriteReflectTypeCPPDeclare(data.NameWithNamespace) << "> ";
				file << typeNameData.Name << "_" << data.Name << ";";
				file << "\n";

				if (!data.Inheritances.empty())
				{
					TAB_N(indent);
					file << "{\n";
				}
				
				for (const Parser::ReflectInheritanceData& d : data.Inheritances)
				{
					writeInheritance(d);
				}

				if (!data.Inheritances.empty())
				{
					TAB_N(indent + 1);
					file << "std::vector<std::unique_ptr<::Reflect::ReflectType>> " << data.Name << "_InheritanceChain;\n";

					for (const Parser::ReflectInheritanceData& d : data.Inheritances)
					{
						TAB_N(indent + 1);
						file << data.Name << "_InheritanceChain.push_back(std::move(";
						file << typeNameData.Name << "_" << d.Name << "));\n";
					}
				}

				TAB_N(indent + 1);
				file << typeNameData.Name << "_" << data.Name << " = ";
				file << "std::make_unique<" << CG_Utils::WriteReflectTypeCPPDeclare(data.NameWithNamespace) << ">";
				file << CG_Utils::WriteReflectTypeCPPParentheses(reflectType, typeNameData.ReflectValueType, data.Inheritances, data.Name) << ";";
				file << "\n";

				if (!data.Inheritances.empty())
				{
					TAB_N(indent);
					file << "}\n";
				}
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

	std::string CG_CPP_Legacy::MemberFormat()
	{
		return "Reflect::ReflectMember(member.Name, member.Type, ((char*)this) + member.Offset)";
	}
}