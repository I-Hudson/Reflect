#include "Reflect/CodeGenerate/Headers/CG_Header_Legacy.h"
#include "Reflect/CodeGenerate/CodeGenerate.h"

#include "Reflect/Core/Core.h"
#include "Reflect/Core/Util.h"

#include "Reflect/Instrumentor.h"

namespace Reflect::CodeGeneration
{
	std::string CG_Header_Legacy::GetType(const Parser::ReflectTypeNameData& arg, bool defaultReturnPointer)
	{
		REFLECT_PROFILE_FUNCTION();
		if (arg.ReflectValueType == Reflect::EReflectValueType::Pointer)
			return arg.Type + "*";
		else if (arg.ReflectValueType == Reflect::EReflectValueType::Reference)
			return arg.Type + "*";
		else if (arg.ReflectValueType == Reflect::EReflectValueType::PointerReference)
			return arg.Type + "*&";
		else
			return defaultReturnPointer ? arg.Type + "*" : arg.Type;
	}

	void CG_Header_Legacy::WriteGeneratedData(const Parser::ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		WriteMemberProperties(data, file, currentFileId, additionalOptions);
		WriteFunctionGet(data, file, currentFileId, additionalOptions);
		WriteMemberPropertiesOffsets(data, file, currentFileId, additionalOptions);
		WriteMemberGet(data, file, currentFileId, additionalOptions);
		WriteFunctions(data, file, currentFileId, additionalOptions);
	}

	void CG_Header_Legacy::WriteClosingMacro(std::ofstream& file, std::string_view currentFileId)
	{
		REFLECT_PROFILE_FUNCTION();
		file << currentFileId;
		file << "_PROPERTIES";
		file << NEW_LINE_SLASH;

		file << currentFileId;
		file << "_PROPERTIES_OFFSET";
		file << NEW_LINE_SLASH;

		file << currentFileId;
		file << "_PROPERTIES_GET";
		file << NEW_LINE_SLASH;

		file << currentFileId;
		file << "_FUNCTION_GET";
		file << NEW_LINE_SLASH;
	}

	void CG_Header_Legacy::WriteMemberProperties(const Parser::ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		file << "#define " + currentFileId + "_PROPERTIES" + NEW_LINE_SLASH;
		WRITE_PRIVATE;
		file << "\tstatic Reflect::ReflectMemberProp __REFLECT_MEMBER_PROPS__[" + std::to_string(data.Members.size()) + "];" + NEW_LINE_SLASH;
		WRITE_CLOSE;
	}

	void CG_Header_Legacy::WriteMemberPropertiesOffsets(const Parser::ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		file << "#define " + currentFileId + "_PROPERTIES_OFFSET" + NEW_LINE_SLASH;
		WRITE_PRIVATE;
		for (const auto& member : data.Members)
		{
			file << "\tstatic size_t __REFLECT__" + member.Name + "() { return offsetof(" + data.Name + ", " + member.Name + "); };" + NEW_LINE_SLASH;
		}
		WRITE_CLOSE;
	}

	void CG_Header_Legacy::WriteMemberGet(const Parser::ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		file << "#define " + currentFileId + "_PROPERTIES_GET \\\n";
		WRITE_PUBLIC;
		file << "\tvirtual Reflect::ReflectMember GetMember(const char* memberName) override;" << NEW_LINE_SLASH;
		file << "\tvirtual std::vector<Reflect::ReflectMember> GetMembers(std::vector<std::string> const& flags) override;" << NEW_LINE_SLASH;
		file << "\tvirtual std::vector<Reflect::ReflectMember> GetAllMembers() override;" << NEW_LINE_SLASH;
		WRITE_CLOSE;
	}

	void CG_Header_Legacy::WriteFunctions(const Parser::ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		//TODO: Pass in parameters in someway. Prob need to use templates.
		auto populateArgs = [](const std::vector<Parser::ReflectTypeNameData>& args) -> std::string
		{
			std::string returnValue;
			for (const auto& arg : args)
			{
				if (arg.ReflectValueType == EReflectValueType::Value)
				{
					returnValue += "*";
				}
				if (arg.ReflectValueType == EReflectValueType::Reference)
				{
					returnValue += "*";
				}

				returnValue += arg.Name + "Arg";
				if (arg != args.back())
				{
					returnValue += ", ";
				}
			}
			return returnValue;
		};
		auto castToType = [](const Reflect::Parser::ReflectTypeNameData& arg) -> std::string
		{
			if (arg.ReflectValueType == Reflect::EReflectValueType::Reference || arg.ReflectValueType == Reflect::EReflectValueType::PointerReference)
			{
				return "static_cast<" + arg.Type + "*>";
			}
			return "static_cast<" + arg.Type + "*>";
		};
		auto returnType = [](const Reflect::Parser::ReflectFunctionData& func) -> std::string
		{
			std::string result;
			if (func.ReflectValueType == EReflectValueType::Value)
				result = "*(" + func.Type + "*)returnValuePtr = ";
			else if (func.ReflectValueType == EReflectValueType::Pointer)
				result = "*((" + func.Type + "**)returnValuePtr) = ";
			else if (func.ReflectValueType == EReflectValueType::Reference)
				result = "*((" + func.Type + "**)returnValuePtr) = &";

			if (func.ReflectValueType != EReflectValueType::Value && func.ReflectModifier == EReflectValueModifier::Const)
				result += "const_cast<" + func.Type + Util::EReflectValueTypeToString(func.ReflectValueType) + ">(";

			return result;
		};

		file << "#define " + currentFileId + "_FUNCTION_DECLARE" + NEW_LINE_SLASH;
		WRITE_PRIVATE;
		for (const auto& func : data.Functions)
		{
			file << "\tstatic Reflect::EReflectReturnCode __REFLECT_FUNC__" + func.Name + "(void* objectPtr, void* returnValuePtr, Reflect::FunctionInfoArgs& functionArgs)" + NEW_LINE_SLASH;
			file << "\t{" << NEW_LINE_SLASH;;
			int functionArgIndex = 0;
			for (const auto& arg : func.Parameters)
			{
				TAB_N(2);
				file << GetType(arg, true) + " " + arg.Name + "Arg = " + castToType(arg) + "(functionArgs.GetArg(" + std::to_string(functionArgIndex++) + ").Get());" + NEW_LINE_SLASH;
			}
			TAB_N(2);
			file << data.Name + "* ptr = static_cast<" + data.Name + "*>(objectPtr);" + NEW_LINE_SLASH;;
			TAB_N(2);
			file << "if (ptr == nullptr) { return Reflect::EReflectReturnCode::CAST_FAILED; }" << NEW_LINE_SLASH;;
			if (func.Type != "void")
			{
				TAB_N(2);
				file << "if (returnValuePtr != nullptr)" << NEW_LINE_SLASH << "\t\t{" << NEW_LINE_SLASH;
				TAB_N(3);
				file << returnType(func) << "ptr->" + func.Name + "(" + populateArgs(func.Parameters) + ");" << NEW_LINE_SLASH "\t\t}" << NEW_LINE_SLASH;
				TAB_N(2);
				file << "else\\\n\t\t{\\\n\t\t\t" << "ptr->" + func.Name + "(" + populateArgs(func.Parameters) + ");" << NEW_LINE_SLASH "\t\t}" << NEW_LINE_SLASH;
			}
			else
			{
				TAB_N(2);
				file << "ptr->" + func.Name + "(" + populateArgs(func.Parameters) + ");" + NEW_LINE_SLASH;
			}

			if (func.ReflectValueType != EReflectValueType::Value && func.ReflectModifier == EReflectValueModifier::Const)
				file << ")";
			TAB_N(2);
			file << "return Reflect::EReflectReturnCode::SUCCESS;" << NEW_LINE_SLASH;
			file << "\t}\\\n";
		}
		WRITE_CLOSE;
	}

	void CG_Header_Legacy::WriteFunctionGet(const Parser::ReflectContainerData& data, std::ofstream& file, const std::string& currentFileId, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		file << "#define " + currentFileId + "_FUNCTION_GET" + NEW_LINE_SLASH;
		WRITE_PUBLIC;
		file << "\tvirtual Reflect::ReflectFunction GetFunction(const char* functionName) override;" << NEW_LINE_SLASH;
		WRITE_CLOSE;
	}

}