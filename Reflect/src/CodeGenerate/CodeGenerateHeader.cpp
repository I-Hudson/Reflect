#include "CodeGenerate/CodeGenerateHeader.h"
#include "CodeGenerate/CodeGenerate.h"
#include <assert.h>

NAMESPACE_START

std::string GetCurrentFileID(const std::string& fileName)
{
	return  fileName + "_Source_h";
}

#define WRITE_CURRENT_FILE_ID(FileName) file << "#define " + GetCurrentFileID(FileName)
#define WRITE_CLOSE() file << "\n\n"

#define WRITE_PUBLIC() file << "public:\\\n"
#define WRITE_PRIVATE() file << "private:\\\n"

void CodeGenerateHeader::GenerateHeader(const FileParsedData& data, std::ofstream& file)
{
	file << " // This file is auto generated please don't modify.\n";

	CodeGenerate::IncludeHeader("Core/ReflectObject.h", file);
	CodeGenerate::IncludeHeader("Core/Util.h", file);

	file << "\n";
	file << "#ifdef " + data.FileName + ReflectFileGeneratePrefix + "_h\n";
	file << "#error \"" + data.FileName + ReflectFileGeneratePrefix + ".h" + " already included, missing 'pragma once' in " + data.FileName + ".h\"\n";
	file << "#endif " + data.FileName + ReflectFileGeneratePrefix + "_h\n";
	file << "#define " + data.FileName + ReflectFileGeneratePrefix + "_h\n\n";

	WriteMacros(data, file);
}

void CodeGenerateHeader::WriteMacros(const FileParsedData& data, std::ofstream& file)
{
	for (const auto& reflectData : data.ReflectData)
	{
		const std::string CurrentFileId = GetCurrentFileID(data.FileName) + "_" + std::to_string(reflectData.ReflectGenerateBodyLine);

		WriteMemberProperties(reflectData, file, CurrentFileId);
		WriteFunctions(reflectData, file, CurrentFileId);
		WriteFunctionGet(reflectData, file, CurrentFileId);
		WriteMemberPropertiesOffsets(reflectData, file, CurrentFileId);
		WriteMemberGet(reflectData, file, CurrentFileId);

		WRITE_CURRENT_FILE_ID(data.FileName) + "_" + std::to_string(reflectData.ReflectGenerateBodyLine) + "_GENERATED_BODY \\\n";
		file << CurrentFileId + "_PROPERTIES \\\n";
		file << CurrentFileId + "_FUNCTION_DECLARE \\\n";
		file << CurrentFileId + "_FUNCTION_GET \\\n";
		file << CurrentFileId + "_PROPERTIES_OFFSET \\\n";
		file << CurrentFileId + "_PROPERTIES_GET \\\n";

		WRITE_CLOSE();
	}

	file << "#undef CURRENT_FILE_ID\n";
	file << "#define CURRENT_FILE_ID " + GetCurrentFileID(data.FileName) + "\n";
}

void CodeGenerateHeader::WriteMemberProperties(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId)
{
	file << "#define " + currentFileId + "_PROPERTIES \\\n";
	WRITE_PRIVATE();
	file << "\tstatic Reflect::ReflectMemberProp __REFLECT_MEMBER_PROPS__["+ std::to_string(data.Members.size()) +"];\\\n";
	WRITE_CLOSE();
}

void CodeGenerateHeader::WriteMemberPropertiesOffsets(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId)
{
	file << "#define " + currentFileId + "_PROPERTIES_OFFSET \\\n";
	WRITE_PRIVATE();
	for (const auto& member : data.Members)
	{
		file << "\tstatic int __REFLECT__" + member.Name + "() { return offsetof(" + data.Name + ", " + member.Name + "); }; \\\n";
	}
	WRITE_CLOSE();
}

void CodeGenerateHeader::WriteMemberGet(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId)
{
	file << "#define " + currentFileId + "_PROPERTIES_GET \\\n";
	WRITE_PUBLIC();
	file << "ReflectMember GetMember(const char* memberName);\\\n";
	WRITE_CLOSE();
}

void CodeGenerateHeader::WriteFunctions(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId)
{
	//TODO: Pass in parameters in someway. Prob need to use templates.

	auto populateArgs = [](const std::vector<ReflectTypeNameData>& args) -> std::string
	{
		std::string returnValue;
		for (const auto& arg :args)
		{
			returnValue += arg.Name + "Arg";
			if (arg != args.back())
			{
				returnValue += ", ";
			}
		}
		return returnValue;
	};

	auto castToType = [](const Reflect::ReflectTypeNameData& arg) -> std::string
	{
		if (arg.ReflectMemberType == Reflect::ReflectMemberType::Pointer)
		{
			return "static_cast<" + arg.Type + ">";
		}
		else if (arg.ReflectMemberType == Reflect::ReflectMemberType::Reference)
		{
			return "*static_cast<" + arg.Type.substr(0, arg.Type.length() - 1) + "*>";
		}
		else
		{
			return "*static_cast<" + arg.Type + "*>";
		}
	};

	file << "#define " + currentFileId + "_FUNCTION_DECLARE \\\n";
	WRITE_PRIVATE();
	for (const auto& func : data.Functions)
	{
		file << "\tstatic void __REFLECT_FUNC__" + func.Name + "(void* objectPtr, void* returnValuePtr, FunctionPtrArgs& functionArgs = FunctionPtrArgs())\\\n";
		file << "\t{\\\n";
		int functionArgIndex = 0;
		for (const auto& arg : func.Parameters)
		{
			file << "\t\t" + arg.Type + " " + arg.Name + "Arg = " + castToType(arg) +"(functionArgs.GetArg(" + std::to_string(functionArgIndex++) +"));\\\n";
		}
		file << "\t\t" + data.Name + "* ptr = static_cast<"+data.Name+"*>(objectPtr);\\\n";
		if (func.Type != "void")
		{
			file << "\t\t*((" + func.Type + "*)returnValuePtr) = ptr->" + func.Name + "(" + populateArgs(func.Parameters) +");\\\n";
		}
		else
		{
			file << "\t\tptr->" + func.Name + "();\\\n";
		}
		file << "\t}\\\n";
	}
	WRITE_CLOSE();
}

void CodeGenerateHeader::WriteFunctionGet(const ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId)
{
	file << "#define " + currentFileId + "_FUNCTION_GET \\\n";
	WRITE_PUBLIC();
	file << "\tReflectFunction GetFunction(const char* functionName);\\\n";
	WRITE_CLOSE();
}

NAMESPACE_END