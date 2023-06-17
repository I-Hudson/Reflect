#include "Structs/FunctionInfo.h"

namespace Reflect
{
    FunctionInfo::FunctionInfo()
    {
    }

    FunctionInfo::FunctionInfo(Type returnType, std::string_view functionName, std::vector<Type> argumentTypes, FunctionInfoPtr functionPtr, void* objectInstance)
        : m_returnType(std::move(returnType))
        , m_functionName(functionName)
        , m_argumentTypes(std::move(argumentTypes))
        , m_functionPtr(functionPtr)
        , m_objectInstance(objectInstance)
    {
    }

    FunctionInfo::~FunctionInfo()
    {
    }

    const TypeId& FunctionInfo::GetRetrunTypeId() const
    {
        return m_returnType.GetTypeId();
    }

    const Type& FunctionInfo::GetReturnType() const
    {
        return m_returnType;
    }

    std::string_view FunctionInfo::GetFunctionName() const
    {
        return m_functionName;
    }

    const std::vector<Type>& FunctionInfo::GetArguementTypes() const
    {
        return m_argumentTypes;
    }

    EReflectReturnCode FunctionInfo::Invoke() const
    {
        FunctionPtrArgs args;
        return InternaInvoke(nullptr, args, Type::MakeType<void>());
    }

    EReflectReturnCode FunctionInfo::Invoke(const FunctionPtrArgs& args) const
    {
        return InternaInvoke(nullptr, args, Type::MakeType<void>());
    }

    EReflectReturnCode FunctionInfo::InternaInvoke(void* returnValue, FunctionPtrArgs functionArgs, const Type& returnType) const
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

    bool FunctionInfo::VerifyArgs(const FunctionPtrArgs& functionArgs) const
    {
        if (m_argumentTypes.size() != static_cast<uint64_t>(functionArgs.GetSize()))
        {
            return false;
        }

        for (size_t i = 0; i < m_argumentTypes.size(); ++i)
        {
            if (m_argumentTypes.at(i).GetTypeName() != functionArgs.GetArg(static_cast<uint32_t>(i)).GetType())
            {
                return false;
            }
        }

        return true;
    }

    bool FunctionInfo::CheckReturnType(const Type& returnType) const
    {
        return m_returnType == returnType;
    }
}