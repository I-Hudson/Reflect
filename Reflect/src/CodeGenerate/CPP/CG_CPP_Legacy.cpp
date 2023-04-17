#include "CodeGenerate/CPP/CG_CPP_Legacy.h"
#include "CodeGenerate/Headers/CG_Header_Legacy.h"
#include "CodeGenerate/CodeGenerate.h"

#include "Core/Core.h"
#include "Instrumentor.h"

namespace Reflect
{
	void CG_CPP_Legacy::Generate(const ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		WriteMemberProperties(data, file, additionalOptions);
		WriteFunctionGet(data, file, additionalOptions);
		WriteMemberGet(data, file, additionalOptions);
	}

	void CG_CPP_Legacy::WriteMemberProperties(const ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions)
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
			file << "Reflect::ReflectMemberProp " + data.Name + "::__REFLECT_MEMBER_PROPS__[" + std::to_string(data.Members.size()) + "] = {" << NEW_LINE;
			for (const auto& member : data.Members)
			{
				file << "\tReflect::ReflectMemberProp(\"" + member.Name +
					"\", new Reflect::ReflectTypeCPP<" + CG_Header_Legacy::GetType(member, false) + ">(Reflect::EReflectType::Member), " +
					"__REFLECT__" + member.Name + "(), " +
					getMemberProps(member.ContainerProps) + ")," << NEW_LINE;
			}
			file << "};\n" << NEW_LINE;
		}
	}

	void CG_CPP_Legacy::WriteMemberGet(const ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		file << "Reflect::ReflectMember " + data.Name + "::GetMember(const char* memberName)\n{" << NEW_LINE;
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

		file << "std::vector<Reflect::ReflectMember> " + data.Name + "::GetMembers(std::vector<std::string> const& flags)\n{" << NEW_LINE;
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

		file << "std::vector<Reflect::ReflectMember> " + data.Name + "::GetAllMembers()\n{" << NEW_LINE;
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

	void CG_CPP_Legacy::WriteFunctionGet(const ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		file << "Reflect::ReflectFunction " + data.Name + "::GetFunction(const char* functionName)\n{" << NEW_LINE;
		for (const auto& func : data.Functions)
		{
			TAB_N(1);
			file << "if(functionName == \"" + func.Name + "\")" << NEW_LINE;
			TAB_N(1);
			file << "{" << NEW_LINE;
			TAB_N(2);
			file << "return Reflect::ReflectFunction(this, " + data.Name + "::__REFLECT_FUNC__" + func.Name + ");" << NEW_LINE;
			TAB_N(1);
			file << "}" << NEW_LINE;
		}
		file << "\treturn __super::GetFunction(functionName);" << NEW_LINE;
		file << "}\n" << NEW_LINE;
	}

	std::string CG_CPP_Legacy::MemberFormat()
	{
		return "Reflect::ReflectMember(member.Name, member.Type, ((char*)this) + member.Offset)";
	}
}