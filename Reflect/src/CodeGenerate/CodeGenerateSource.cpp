#include "CodeGenerate/CodeGenerateSource.h"
#include "CodeGenerate/CodeGenerate.h"

NAMESPACE_START

void CodeGenerateSource::GenerateSource(const FileParsedData& data, std::ofstream& file)
{
	//CodeGenerate::IncludeHeader(data.FileName + ReflectFileGeneratePrefix + ".h", file);
	CodeGenerate::IncludeHeader("../" + data.FileName + ".h", file);
	file << "\n";

	for (auto& reflectData : data.ReflectData)
	{
		WriteMemberProperties(reflectData, file);
		WriteFunctionGet(reflectData, file);
		WriteMemberGet(reflectData, file);
	}
}

void CodeGenerateSource::WriteMemberProperties(const ReflectContainerData& data, std::ofstream& file)
{
	auto getMemberProps = [](const std::vector<ReflectFlags>& flags) -> std::string
	{
		int flag = 0;
		for (const auto& f : flags )
		{
			flag |= static_cast<int>(f);
		}
		return std::to_string(flag);
	};

	file << "Reflect::ReflectMemberProp "+ data.Name +"::__REFLECT_MEMBER_PROPS__[" + std::to_string(data.Members.size()) + "] = {\n";
	for (const auto& member : data.Members)
	{
		file << "\tReflect::ReflectMemberProp(\""+ member.Name + "\", Reflect::Util::GetTypeName<" + member.Type + ">(), __REFLECT__" + member.Name +"(), " + getMemberProps(member.ContainerProps) + "),\n";
	}
	file << "};\n\n";
}

void CodeGenerateSource::WriteMemberGet(const ReflectContainerData& data, std::ofstream& file)
{
	file << "ReflectMember " + data.Name + "::GetMember(const char* memberName)\n{\n";
	file << "\tfor(const auto& member : __REFLECT_MEMBER_PROPS__)\n\t{\n";
	file << "\t\tif(memberName == member.Name)\n";
	file << "\t\t{\n";
	file << "\t\t\t//CheckFlags\n";
	file << "\t\t\treturn ReflectMember(member.Name, member.Type, ((char*)this) + member.Offset);\n";
	file << "\t\t}\n";
	file << "\t}\n";
	file << "\treturn ReflectMember(\"null\", Reflect::Util::GetTypeName<void>(), nullptr);\n";
	file << "}\n\n";

	file << "std::vector<ReflectMember> " + data.Name + "::GetMembers(unsigned int flags)\n{\n";
	file << "\tstd::vector<ReflectMember> members;\n";
	file << "\tfor(const auto& member : __REFLECT_MEMBER_PROPS__)\n\t{\n";
	file << "\t\tif(flags & member.Properties)\n";
	file << "\t\t{\n";
	file << "\t\t\tmembers.push_back(ReflectMember(member.Name, member.Type, ((char*)this) + member.Offset));\n";
	file << "\t\t}\n";
	file << "\t}\n";
	file << "\treturn members;\n";
	file << "}\n\n";
}

void CodeGenerateSource::WriteFunctionGet(const ReflectContainerData& data, std::ofstream& file)
{
	file << "ReflectFunction " + data.Name + "::GetFunction(const char* functionName)\n{\n";
	for (const auto& func : data.Functions)
	{
		file << "\tif(functionName == \"" + func.Name + "\")\n";
		file << "\t{\n";
		file << "\t\treturn ReflectFunction(this, " + data.Name +"::__REFLECT_FUNC__" + func.Name +");\n";
		file << "\t}\n";
	}
	file << "\treturn ReflectFunction(nullptr, nullptr);\n";
	file << "}\n\n";
}

//void CodeGenerateSource::WriteFunctionBindings(const ReflectContainerData& data, std::ofstream& file)
//{
//	file << "\t" + data.Name << "* ptr = dynamic_cast<" + data.Name + "*>(this);\n";
//	file << "\tassert(ptr != nullptr && \"[" + data.Name + ContainerPrefix + "::" + "SetupReflectBindings()] 'ptr' should not be null.\");\n\n";
//	for (auto& func : data.Functions)
//	{
//		// Write to the auto generated file binding a function then setting up a "FuncWrapper" for which we can then call the function from.
//		file << "\tauto " + func.Name + "Func = std::bind(&" + data.Name + "::" + func.Name + ", ptr);\n";
//		file << "\tReflect::FuncWrapper " + func.Name + "Wrapper(" + func.Name + "Func);\n";
//		file << "\t__super::AddFunction(" + func.Name + ", " + func.Name + "Wrapper);\n";
//	}
//	file << "\n";
//}

NAMESPACE_END