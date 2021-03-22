#include "CodeGenerateSource.h"
#include "CodeGenerate/CodeGenerate.h"

NAMESPACE_START

void CodeGenerateSource::GenerateSource(const FileParsedData& data, std::ofstream& file)
{
	//CodeGenerate::IncludeHeader(data.FileName + ReflectFileGeneratePrefix + ".h", file);
	CodeGenerate::IncludeHeader("../" + data.FileName + ".h", file);
	file << "\n";

	for (auto& reflectData : data.ReflectData)
	{
		WriteMemberGet(reflectData, file);
	}
}

void CodeGenerateSource::WriteMemberGet(const ReflectContainerData& data, std::ofstream& file)
{
	file << "ReflectMember " + data.Name + "::GetMember(const char* memberName)\n{\n";
	for (const auto& member : data.Members)
	{
		file << "\tif(memberName == \"" + member.Name + "\")\n";
		file << "\t{\n";
		file << "\t\treturn ReflectMember(\"" + member.Name + "\", Reflect::Util::GetTypeName(" + member.Name + "), &" + member.Name + ");\n";
		file << "\t}\n";
	}
	file << "\treturn ReflectMember(\"null\", Reflect::Util::GetTypeName<void>(), nullptr);\n";
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