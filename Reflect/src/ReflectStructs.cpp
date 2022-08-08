#include "ReflectStructs.h"

namespace Reflect
{
#ifdef REFLET_TYPE_INFO
	ReflectTypeFunction::ReflectTypeFunction(void* ownerClass, FunctionPtr funcPtr
		, std::unique_ptr<ReflectType> info, std::vector<std::unique_ptr<ReflectType>> args)
		: m_ownerClass(std::move(ownerClass)), m_func(std::move(funcPtr))
		, m_info(std::move(info)), m_argsInfo(std::move(args))
	{ }

	Reflect::EReflectReturnCode ReflectTypeFunction::Invoke(void* returnValue, FunctionPtrArgs functionArgs)
	{
		if (IsValid())
		{
			return CallInternal((void*)returnValue, std::move(functionArgs));
		}
		return EReflectReturnCode::INVALID_FUNCTION_POINTER;
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

	EReflectReturnCode ReflectTypeFunction::CallInternal(void* returnValue, FunctionPtrArgs functionArgs)
	{
		return m_func(m_ownerClass, returnValue, functionArgs);
	}



	RefectTypeInfo::RefectTypeInfo(void* ownerClass, std::unique_ptr<ReflectType> info, std::vector<std::unique_ptr<ReflectTypeFunction>> functions)
		: m_ownerClass(ownerClass), m_info(std::move(info)), m_functions(std::move(functions))
	{ }

	ReflectType* RefectTypeInfo::GetInfo() const
	{
		return m_info.get();
	}

	ReflectTypeFunction* RefectTypeInfo::GetFunction(const char* functionName) const
	{
		for (const auto& func : m_functions)
		{
			if (func->GetInfo()->GetValueTypeName() == functionName) 
			{
				return func.get();
			}
		}
		return nullptr;
	}
#endif
}