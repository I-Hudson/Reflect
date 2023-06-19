#include "Structs/FunctionInfo.h"

#include <unordered_set>

namespace Reflect
{
    FunctionInfo::FunctionInfo()
    {
    }

    FunctionInfo::FunctionInfo(Type returnType, std::string_view functionName, std::vector<Type> argumentTypes, std::vector<std::string> flags, FunctionInfoPtr functionPtr, void* objectInstance)
        : m_returnType(std::move(returnType))
        , m_functionName(functionName)
        , m_argumentTypes(std::move(argumentTypes))
        , m_flags(std::move(flags))
        , m_functionPtr(functionPtr)
        , m_objectInstance(objectInstance)
    {
    }

    FunctionInfo::~FunctionInfo()
    {
    }

    FunctionInfo::operator bool() const
    {
        return IsValid();
    }

    bool FunctionInfo::IsValid() const
    {
        return m_returnType.IsValid()
            && !m_functionName.empty()
            && m_functionPtr != nullptr
            && m_objectInstance != nullptr;
    }

    TypeId FunctionInfo::GetRetrunTypeId() const
    {
        return m_returnType.GetTypeId();
    }

    Type FunctionInfo::GetReturnType() const
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

    bool FunctionInfo::HasFlag(std::string_view flag) const
    {
        for (const std::string& str : m_flags)
        {
            if (str == flag)
            {
                return true;
            }
        }
        return false;
    }

    bool FunctionInfo::HasAnyFlags(const std::vector<std::string>& flags) const
    {
        std::unordered_set<std::string_view> flagsSet;
        for (const std::string& functionFlag : m_flags)
        {
            flagsSet.insert(functionFlag);
        }

        for (const std::string& flag : flags)
        {
            if (flagsSet.find(flag) != flagsSet.end())
            {
                return true;
            }
        }
        return false;
    }

    const std::vector<std::string>& FunctionInfo::GetFlags() const
    {
        return m_flags;
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
        return m_returnType == returnType;
    }
}