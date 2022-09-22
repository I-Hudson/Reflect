#include "ReflectStructs.h"

namespace Reflect
{
	bool Reflect::ReflectType::IsClass() const
	{
		return m_eReflectType == EReflectType::Class;
	}

	bool ReflectType::IsStruct() const
	{
		return m_eReflectType == EReflectType::Struct;
	}

	bool ReflectType::IsMember() const
	{
		return m_eReflectType == EReflectType::Member;
	}

	bool ReflectType::IsFunction() const
	{
		return m_eReflectType == EReflectType::Function;
	}

	bool ReflectType::IsParameter() const
	{
		return m_eReflectType == EReflectType::Parameter;
	}


#ifdef REFLET_TYPE_INFO
	ReflectTypeMember::ReflectTypeMember(void* ownerClass, void* memberPtr, std::unique_ptr<ReflectType> type
	, std::vector<std::string> flags)
		: m_ownerClass(ownerClass), m_memberPtr(memberPtr), m_type(std::move(type))
		, m_flags(std::move(flags))
	{ }

	ReflectType* ReflectTypeMember::GetType() const
	{
		return m_type.get();
	}

	bool ReflectTypeMember::IsValid() const
	{
		return m_ownerClass && m_memberPtr;
	}

	bool ReflectTypeMember::HasFlag(const char* flag) const
	{
		return std::find(m_flags.begin(), m_flags.end(), flag) != m_flags.end();
	}


	ReflectTypeFunction::ReflectTypeFunction(void* ownerClass, FunctionPtr funcPtr
		, std::unique_ptr<ReflectType> info, std::vector<std::unique_ptr<ReflectType>> args)
		: m_ownerClass(std::move(ownerClass)), m_func(std::move(funcPtr))
		, m_info(std::move(info)), m_argsInfo(std::move(args))
	{
		m_numOfArgs = static_cast<int>(m_argsInfo.size());
	}

	Reflect::EReflectReturnCode ReflectTypeFunction::Invoke(FunctionPtrArgs functionArgs)
	{
		return CallInternal(nullptr, nullptr, std::move(functionArgs));
	}

	bool ReflectTypeFunction::IsValid() const
	{
		return m_ownerClass != nullptr;
	}

	ReflectType* ReflectTypeFunction::GetInfo() const
	{
		return m_info.get();
	}

	const ReflectType* ReflectTypeFunction::GetArgInfo(int index) const
	{
		if (index >= 0 && index < m_numOfArgs)
		{
			return m_argsInfo.at(index).get();
		}
		return nullptr;
	}

	std::vector<ReflectType*> ReflectTypeFunction::GetArgsInfo() const
	{
		std::vector<ReflectType*> args;
		args.reserve(m_numOfArgs);
		std::transform(m_argsInfo.begin(), m_argsInfo.end(), std::back_inserter(args), [](const std::unique_ptr<ReflectType>& reflectType)
			{
				return reflectType.get();
			});
		return args;
	}

	EReflectReturnCode ReflectTypeFunction::CallInternal(void* returnValue, std::unique_ptr<ReflectType> returnType, FunctionPtrArgs functionArgs)
	{
		if (!IsValid())
		{
			return EReflectReturnCode::INVALID_FUNCTION_POINTER;
		}
		if (!VerifyArgs(functionArgs))
		{
			return EReflectReturnCode::FUNCTION_INVALID_ARGS;
		}
		if (!CheckReturnType(returnType.get()))
		{
			return EReflectReturnCode::FUNCTION_INVALID_RETURN_TYPE;
		}
		return m_func(m_ownerClass, returnValue, functionArgs);
	}

	bool ReflectTypeFunction::VerifyArgs(const FunctionPtrArgs& functionArgs) const
	{ 
		if (m_numOfArgs != functionArgs.GetSize())
		{
			return false;
		}

		for (size_t i = 0; i < m_numOfArgs; ++i)
		{
			if (m_argsInfo.at(i).get()->GetTypeName() != functionArgs.GetArg(i).GetType())
			{
				return false;
			}
		}

		return true;
	}

	bool ReflectTypeFunction::CheckReturnType(ReflectType* returnType) const
	{
		if (returnType == nullptr)
		{
			return true;
		}
		return m_info->GetTypeName() == returnType->GetTypeName();
	}

	ReflectTypeInfo::ReflectTypeInfo(void* owner_class, std::unique_ptr<ReflectType> info
		, std::vector<std::unique_ptr<ReflectTypeMember>> members
		, std::vector<std::unique_ptr<ReflectTypeFunction>> functions)
		: m_owner_class(owner_class), m_info(std::move(info))
		, m_members(std::move(members)), m_functions(std::move(functions))
	{ }

	ReflectType* ReflectTypeInfo::GetInfo() const
	{
		return m_info.get();
	}

	ReflectTypeMember* ReflectTypeInfo::GetMember(const char* memberName) const
	{
		for (const auto& member : m_members)
		{
			if (member->GetType()->GetGivenName() == memberName)
			{
				return member.get();
			}
		}
		return nullptr;
	}

	std::vector<ReflectTypeMember*> ReflectTypeInfo::GetAllMembers() const
	{
		std::vector<ReflectTypeMember*> vec;
		std::transform(m_members.begin(), m_members.end(), std::back_inserter(vec),
			[](const std::unique_ptr<Reflect::ReflectTypeMember>& member)
			{
				return member.get();
			});
		return vec;
	}

	std::vector<ReflectTypeMember*> ReflectTypeInfo::GetAllMembersWithFlags(std::vector<const char*> flags) const
	{
		std::vector<ReflectTypeMember*> vec;
		std::for_each(m_members.begin(), m_members.end(), [&vec, flags](const std::unique_ptr<Reflect::ReflectTypeMember>& member)
			{
				for (const auto& flag : flags)
				{
					if (member->HasFlag(flag))
					{
						vec.push_back(member.get());
					}
				}
			});
		return vec;
	}

	ReflectTypeFunction* ReflectTypeInfo::GetFunction(const char* functionName) const
	{
		for (const auto& func : m_functions)
		{
			if (func->GetInfo()->GetGivenName() == functionName) 
			{
				return func.get();
			}
		}
		return nullptr;
	}
#endif
}