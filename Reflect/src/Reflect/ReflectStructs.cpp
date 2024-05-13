#include "Reflect/ReflectStructs.h"

namespace Reflect
{
#ifdef REFLECT_TYPE_INFO_ENABLED
#else
	std::vector<ReflectType*> ReflectType::GetInheritances() const
	{
		std::vector<ReflectType*> vec; vec.reserve(m_inheritanceTypes.size());
		for (const auto& type : m_inheritanceTypes)
		{
			vec.push_back(type.get());
		}
		return vec;
	}

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

	//-------------------------------------
	// ReflectTypeMember
	//-------------------------------------
	ReflectTypeMember::ReflectTypeMember(void* ownerClass, const char* name, void* memberPtr, std::unique_ptr<ReflectType> type
	, std::vector<std::string> flags)
		: m_ownerClass(ownerClass)
		, m_name(name)
		, m_memberPtr(memberPtr)
		, m_type(std::move(type))
		, m_flags(std::move(flags))
	{ }

	ReflectType* ReflectTypeMember::GetType() const
	{
		return m_type.get();
	}

	std::string_view ReflectTypeMember::GetName() const
	{
		return m_name;
	}

	bool ReflectTypeMember::IsValid() const
	{
		return m_ownerClass && m_memberPtr;
	}

	bool ReflectTypeMember::HasFlag(const char* flag) const
	{
		return std::find(m_flags.begin(), m_flags.end(), flag) != m_flags.end();
	}

	void* ReflectTypeMember::GetData() const
	{
		return m_memberPtr;
	}

	//-------------------------------------
	// ReflectTypeFunction
	//-------------------------------------
	ReflectTypeFunction::ReflectTypeFunction(void* ownerClass, const char* name, FunctionPtr funcPtr
		, std::unique_ptr<ReflectType> info, std::vector<std::unique_ptr<ReflectType>> args)
		: m_ownerClass(std::move(ownerClass))
		, m_name(name)
		, m_func(std::move(funcPtr))
		, m_info(std::move(info))
		, m_argsInfo(std::move(args))
	{
		m_numOfArgs = static_cast<int>(m_argsInfo.size());
	}

	Reflect::EReflectReturnCode ReflectTypeFunction::Invoke(FunctionPtrArgs functionArgs)
	{
		return CallInternal(nullptr, std::move(functionArgs), ReflectTypeCPP<void>(EReflectType::Unknown, EReflectValueType::Unknown, { }));
	}

	bool ReflectTypeFunction::IsValid() const
	{
		return VerifyOwnerObject() && VerifyFunctionPointer();
	}

	ReflectType* ReflectTypeFunction::GetInfo() const
	{
		return m_info.get();
	}

	std::string_view ReflectTypeFunction::GetName() const
	{
		return m_name;
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

	EReflectReturnCode ReflectTypeFunction::CallInternal(void* returnValue, FunctionPtrArgs functionArgs, const ReflectType& returnType)
	{
		if (!VerifyOwnerObject())
		{
			return EReflectReturnCode::INVALID_OWNER_OBJECT;
		}
		if (!VerifyFunctionPointer())
		{
			return EReflectReturnCode::INVALID_FUNCTION_POINTER;
		}
		if (!VerifyArgs(functionArgs))
		{
			return EReflectReturnCode::FUNCTION_INVALID_ARGS;
		}
		if (!CheckReturnType(returnType))
		{
			return EReflectReturnCode::FUNCTION_INVALID_RETURN_TYPE;
		}
		return m_func(m_ownerClass, returnValue, functionArgs);
	}

	bool ReflectTypeFunction::VerifyOwnerObject() const
	{
		return m_ownerClass != nullptr;
	}

	bool ReflectTypeFunction::VerifyFunctionPointer() const
	{
		return m_func != nullptr;
	}

	bool ReflectTypeFunction::VerifyArgs(const FunctionPtrArgs& functionArgs) const
	{ 
		if (m_numOfArgs != functionArgs.GetSize())
		{
			return false;
		}

		for (size_t i = 0; i < m_numOfArgs; ++i)
		{
			if (m_argsInfo.at(i).get()->GetTypeName() != functionArgs.GetArg(static_cast<uint32_t>(i)).GetType())
			{
				return false;
			}
		}

		return true;
	}

	bool ReflectTypeFunction::CheckReturnType(const ReflectType& returnType) const
	{
		if (returnType.GetTypeName() == "void")
		{
			return true;
		}
		return m_info->GetTypeName() == returnType.GetTypeName();
	}

	//-------------------------------------
	// ReflectTypeInfo
	//-------------------------------------
	ReflectTypeInfo::ReflectTypeInfo(ReflectTypeId typeId
		, void* owner_class
		, std::unique_ptr<ReflectType> info
		, std::vector<std::unique_ptr<ReflectTypeInfo>> inheritances
		, std::vector<std::unique_ptr<ReflectTypeMember>> members
		, std::vector<std::unique_ptr<ReflectTypeFunction>> functions)
		: m_typeId(std::move(typeId))
		, m_owner_class(owner_class)
		, m_info(std::move(info))
		, m_inheritances(std::move(inheritances))
		, m_members(std::move(members))
		, m_functions(std::move(functions))
	{ }

	ReflectTypeId ReflectTypeInfo::GetTypeId() const
	{
		return m_typeId;
	}

	ReflectType* ReflectTypeInfo::GetInfo() const
	{
		return m_info.get();
	}

	bool ReflectTypeInfo::HasOwner() const
	{
		return m_owner_class != nullptr;
	}

	ReflectTypeMember* ReflectTypeInfo::GetMember(const char* memberName) const
	{
		return GetMember(memberName, false);
	}
	ReflectTypeMember* ReflectTypeInfo::GetMember(const char* memberName, bool includeBaseClasses) const
	{
		for (const auto& member : m_members)
		{
			if (member->GetName() == memberName)
			{
				return member.get();
			}
		}

		if (includeBaseClasses)
		{
			for (const auto& item : m_inheritances)
			{
				ReflectTypeMember* member = item->GetMember(memberName, includeBaseClasses);
				if (member)
				{
					return member;
				}
			}
		}

		return nullptr;
	}

	std::vector<ReflectTypeMember*> ReflectTypeInfo::GetAllMembers() const
	{
		return GetAllMembers(false);
	}
	std::vector<ReflectTypeMember*> ReflectTypeInfo::GetAllMembers(bool includeBaseClasses) const
	{
		std::vector<ReflectTypeMember*> vec;
		std::transform(m_members.begin(), m_members.end(), std::back_inserter(vec),
			[](const std::unique_ptr<Reflect::ReflectTypeMember>& member)
			{
				return member.get();
			});

		if (includeBaseClasses)
		{
			for (const auto& item : m_inheritances)
			{
				std::vector<ReflectTypeMember*> baseMembers = item->GetAllMembers(includeBaseClasses);
				vec.insert(vec.end(), baseMembers.begin(), baseMembers.end());
			}
		}

		return vec;
	}

	std::vector<ReflectTypeMember*> ReflectTypeInfo::GetAllMembersWithFlags(std::vector<const char*> flags) const
	{
		return GetAllMembersWithFlags(std::move(flags), false);
	}
	std::vector<ReflectTypeMember*> ReflectTypeInfo::GetAllMembersWithFlags(std::vector<const char*> flags, bool includeBaseClasses) const
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

		if (includeBaseClasses)
		{
			for (const auto& item : m_inheritances)
			{
				std::vector<ReflectTypeMember*> baseMembers = item->GetAllMembersWithFlags(flags, includeBaseClasses);
				vec.insert(vec.end(), baseMembers.begin(), baseMembers.end());
			}
		}

		return vec;
	}

	ReflectTypeFunction* ReflectTypeInfo::GetFunction(const char* functionName) const
	{
		return GetFunction(functionName, false);
	}
	ReflectTypeFunction* ReflectTypeInfo::GetFunction(const char* functionName, bool includeBaseClasses) const
	{
		for (const auto& func : m_functions)
		{
			if (func->GetName() == functionName)
			{
				return func.get();
			}
		}

		if (includeBaseClasses)
		{
			for (const auto& item : m_inheritances)
			{
				ReflectTypeFunction* func = item->GetFunction(functionName, includeBaseClasses);
				if (func)
				{
					return func;
				}
			}
		}

		return nullptr;
	}


	//-------------------------------------
	// ReflectTypeInfoRegisty
	//-------------------------------------
	ReflectTypeInfoRegisty::ReflectTypeInfoRegisty()
	{
	}

	ReflectTypeInfoRegisty::~ReflectTypeInfoRegisty()
	{
		std::lock_guard lock(Instance().m_registyLock);
		Instance().m_registy.clear();
	}

	void ReflectTypeInfoRegisty::RegisterTypeInfo(ReflectTypeId typeId, CreateTypeInfoFunc createTypeInfoFunc)
	{
		//std::lock_guard lock(s_registyLock);

		if (auto iter = Instance().m_registy.find(typeId);
			iter != Instance().m_registy.end())
		{
			return;
		}
		Instance().m_registy[typeId] = createTypeInfoFunc;
	}

	void ReflectTypeInfoRegisty::UnregisterTypeInfo(ReflectTypeId typeId)
	{
		std::lock_guard lock(Instance().m_registyLock);

		if (auto iter = Instance().m_registy.find(typeId);
			iter != Instance().m_registy.end())
		{
			Instance().m_registy.erase(typeId);
		}

	}

	ReflectTypeInfo ReflectTypeInfoRegisty::GetTypeInfo(const ReflectTypeId& typeId)
	{
		return GetTypeInfo(typeId, nullptr);
	}

	ReflectTypeInfo ReflectTypeInfoRegisty::GetTypeInfo(const ReflectTypeId& typeId, void* objectInstance)
	{
		std::lock_guard lock(Instance().m_registyLock);
		if (auto iter = Instance().m_registy.find(typeId);
			iter != Instance().m_registy.end())
		{
			return iter->second(objectInstance);
		}
		return ReflectTypeInfo(ReflectTypeId(""), nullptr, {}, {}, {}, {});
	}
#endif
}