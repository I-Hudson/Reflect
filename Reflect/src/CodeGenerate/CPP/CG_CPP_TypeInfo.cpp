#include "CodeGenerate/CPP/CG_CPP_TypeInfo.h"
#include "CodeGenerate/CodeGenerate.h"

#include "Core/Core.h"

#include "Instrumentor.h"

namespace Reflect
{
#ifdef REFLECT_TYPE_INFO_ENABLED

    void CG_CPP_TypeInfo::Generate(const ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions)
    {
		REFLECT_PROFILE_FUNCTION();
        WriteGenerateTypeInfo(data, file, additionalOptions);
    }

	void CG_CPP_TypeInfo::WriteGenerateTypeInfo(const ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		file << "template<>" << NEW_LINE;
		file << "class Reflect::GenerateTypeInfoForType<" + data.Name + ">\n{" << NEW_LINE;
		file << "public:" << NEW_LINE;
		file << "\tReflect::ReflectTypeInfo GetTypeInfo(" + data.Name + "* ownerClass)\n\t{" << NEW_LINE;
		file << "\t\tstd::vector<std::unique_ptr<Reflect::ReflectTypeInfo>> inheritances = GenerateInheritance(ownerClass);" << NEW_LINE;
		file << "\t\tstd::vector<std::unique_ptr<Reflect::ReflectTypeFunction>> functions = GenerateFunctions(ownerClass);" << NEW_LINE;
		file << "\t\tstd::vector<std::unique_ptr<Reflect::ReflectTypeMember>> members = GenerateMembers(ownerClass);" << NEW_LINE;
		file << "\t\tReflectTypeInfo reflect_type_info = ReflectTypeInfo(ownerClass," << NEW_LINE;
		file << "\t\t\tstd::make_unique<ReflectTypeCPP<" + data.Name + ">>(Reflect::EReflectType::Class)," << NEW_LINE;
		file << "\t\t\tstd::move(inheritances), " << NEW_LINE;
		file << "\t\t\tstd::move(members), " << NEW_LINE;
		file << "\t\t\tstd::move(functions));" << NEW_LINE;
		file << "\t\treflect_type_info.m_construct_func = [](){ return static_cast<void*>(new " + data.Name + "); }; " << NEW_LINE;
		file << "\t\treturn reflect_type_info; " << NEW_LINE;
		file << "\t}" << NEW_LINE;
		file << "private:\n";

		WriteGenerateTypeInheritance(data, file, additionalOptions);
		WriteGenerateTypeMembers(data, file, additionalOptions);
		WriteGenerateTypeFunctions(data, file, additionalOptions);

		file << "};\n" << NEW_LINE;

		file << "Reflect::ReflectTypeInfo " + data.Name + "::GetTypeInfo()\n{" << NEW_LINE;
		file << "\treturn Reflect::GenerateTypeInfoForType<" + data.Name + ">().GetTypeInfo(nullptr);" << NEW_LINE;
		file << "}\n" << NEW_LINE;
		file << "Reflect::ReflectTypeInfo " + data.Name + "::GetTypeInfo(" + data.Name + "* classPtr)\n{" << NEW_LINE;
		file << "\treturn Reflect::GenerateTypeInfoForType<" + data.Name + ">().GetTypeInfo(classPtr);" << NEW_LINE;
		file << "}\n" << NEW_LINE;
	}

	void CG_CPP_TypeInfo::WriteGenerateTypeInheritance(const ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		file << "\tstd::vector<std::unique_ptr<Reflect::ReflectTypeInfo>> GenerateInheritance(" + data.Name + "* ownerClass)\n\t{" << NEW_LINE;
		file << "\t\tstd::vector<std::unique_ptr<Reflect::ReflectTypeInfo>> inheritances;\n" << NEW_LINE;

		for (const auto& item : data.Inheritance)
		{
			if (!item.IsReflected) 
			{
				continue;
			}
			file << TAB << TAB << item.Name + "* " + item.Name + "_pointer = dynamic_cast<" + item.Name + "*>(ownerClass);" << NEW_LINE;
			//file << TAB << TAB << "assert(" + item.Name + "_pointer);" << NEW_LINE;
			file << TAB << TAB << "inheritances.push_back(std::make_unique<Reflect::ReflectTypeInfo>(" + item.Name + "::GetTypeInfo(" + item.Name + "_pointer != nullptr ? " + item.Name + "_pointer : nullptr)));" << NEW_LINE;
		}

		file << NEW_LINE;
		file << "\t\treturn inheritances;" << NEW_LINE;
		file << "\t}" << NEW_LINE;
	}

	void CG_CPP_TypeInfo::WriteGenerateTypeMembers(const ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		file << "\tstd::vector<std::unique_ptr<Reflect::ReflectTypeMember>> GenerateMembers(" + data.Name + "* ownerClass)\n\t{" << NEW_LINE;
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
			file << "(unsigned char*)ownerClass + offsetof(" + data.Name + ", " + member.Name + "), ";
			file << "std::make_unique<ReflectTypeCPP<" + member.RawType + ">>(Reflect::EReflectType::Member, \"" + member.Name + "\"), ";
			file << "flags";
			file << ")); " << NEW_LINE << NEW_LINE;
		}
		file << "\t\treturn members;" << NEW_LINE;
		file << "\t}" << NEW_LINE;
	}

	void CG_CPP_TypeInfo::WriteGenerateTypeFunctions(const ReflectContainerData& data, std::ofstream& file, const ReflectAddtionalOptions* additionalOptions)
	{
		REFLECT_PROFILE_FUNCTION();
		auto generateFunctionArgs = [&data]()->std::string
		{
			auto generateSingleArg = [&data](const ReflectTypeNameData& arg) -> std::string
			{
				std::string str;
				str += "std::make_unique<ReflectTypeCPP<" + arg.RawType + ">>(Reflect::EReflectType::Parameter, \"" + arg.Name + "\")";
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

		auto generateSingleFunction = [&data](const ReflectFunctionData& func) -> std::string
		{
			std::string str;
			str += "std::make_unique<Reflect::ReflectTypeFunction>";
			str += "((void*)ownerClass, ";
			str += "\"" + func.Name + "\"" + ", ";
			str += data.Name + "::__REFLECT_FUNC__" + func.Name + ", ";
			str += "std::make_unique<ReflectTypeCPP<" + func.Type + ">>(Reflect::EReflectType::Function, \"" + func.Name + "\"), ";
			str += "std::move(" + func.Name + "_Args))";
			return str;
		};

		file << "\tstd::vector<std::unique_ptr<Reflect::ReflectTypeFunction>> GenerateFunctions(" + data.Name + "* ownerClass)\n\t{" << NEW_LINE;

		file << generateFunctionArgs() + NEW_LINE;

		file << "\t\tstd::vector<std::unique_ptr<Reflect::ReflectTypeFunction>> funcs;" << NEW_LINE;
		for (const auto& f : data.Functions)
		{
			file << "\t\tfuncs.push_back(" + generateSingleFunction(f) + ");" << NEW_LINE;
		}
		file << "\t\treturn funcs;" << NEW_LINE;
		file << "\t}" << NEW_LINE;
	}
#endif
}