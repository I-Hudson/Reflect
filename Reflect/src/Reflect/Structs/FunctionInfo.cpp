#include "Reflect/Structs/FunctionInfo.h"

#include <unordered_set>

namespace Reflect
{
    FunctionInfo::FunctionInfo()
    {
    }

    FunctionInfo::FunctionInfo(Type returnType, std::string_view functionName, std::vector<Type> argumentTypes, std::vector<std::string> flags, FunctionInfoPtr functionPtr, void* objectInstance)
        : Property(std::move(returnType), std::string(functionName), PropertyType::Function, std::move(flags), {}, objectInstance)
        , m_argumentTypes(std::move(argumentTypes))
        , m_functionPtr(functionPtr)
    {
    }

    FunctionInfo::FunctionInfo(Type returnType, std::string_view functionName, std::vector<Type> argumentTypes, std::vector<std::string> flags, std::vector<PropertyMeta> propertyMetas, FunctionInfoPtr functionPtr, void* objectInstance)
        : Property(std::move(returnType), std::string(functionName), PropertyType::Function, std::move(flags), {}, objectInstance)
        , m_argumentTypes(std::move(argumentTypes))
        , m_functionPtr(functionPtr)
    { }

    FunctionInfo::~FunctionInfo()
    {
    }


    bool FunctionInfo::IsValid() const
    {
        return Property::IsValid()
            && m_functionPtr != nullptr;
    }

    const std::vector<Type>& FunctionInfo::GetArguementTypes() const
    {
        return m_argumentTypes;
    }

    EReflectReturnCode FunctionInfo::Invoke() const
    {
        FunctionInfoArgs args;
        return InternaInvoke(nullptr, args, Type::MakeType<void>());
    }

    EReflectReturnCode FunctionInfo::Invoke(const FunctionInfoArgs& args) const
    {
        return InternaInvoke(nullptr, args, Type::MakeType<void>());
    }

    EReflectReturnCode FunctionInfo::InternaInvoke(void* returnValue, FunctionInfoArgs functionArgs, const Type& returnType) const
    {
        if (!VerifyObjectInstance())
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
        return m_functionPtr(m_objectInstance, returnValue, functionArgs);
    }

    bool FunctionInfo::VerifyObjectInstance() const
    {
        return m_objectInstance != nullptr;;
    }

    bool FunctionInfo::VerifyFunctionPointer() const
    {
        return m_functionPtr != nullptr;
    }

    bool FunctionInfo::VerifyArgs(const FunctionInfoArgs& functionArgs) const
    {
        if (m_argumentTypes.size() != static_cast<uint64_t>(functionArgs.GetSize()))
        {
            return false;
        }

        for (size_t i = 0; i < m_argumentTypes.size(); ++i)
        {
            if (m_argumentTypes.at(i) != functionArgs.GetArg(static_cast<uint32_t>(i)).GetType())
            {
                return false;
            }
        }

        return true;
    }

    bool FunctionInfo::CheckReturnType(const Type& returnType) const
    {
        return m_objectType == returnType;
    }

    void FunctionInfo::SetObjectInstance(void* objectInstance)
    {
        m_objectInstance = objectInstance;
    }
}