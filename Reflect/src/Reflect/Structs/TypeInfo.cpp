#include "Reflect/Structs/TypeInfo.h"

#include <unordered_set>
#include <iterator>

namespace Reflect
{
    TypeInfo::TypeInfo()
    {
    }

    TypeInfo::TypeInfo(Type type, void* objectInstance, std::vector<TypeInfo> parentInfos, std::vector<MemberInfo> memberInfos, std::vector<FunctionInfo> functionInfos)
        : m_type(std::move(type))
        , m_objectInstance(objectInstance)
        , m_parentTypeInfos(std::move(parentInfos))
        , m_memberInfos(std::move(memberInfos))
        , m_functionInfos(std::move(functionInfos))
    {
    }

    TypeInfo::~TypeInfo()
    {
    }

    TypeInfo::operator bool() const
    {
        return IsValid();
    }

    bool TypeInfo::IsValid() const
    {
        return m_type.IsValid()
            && m_objectInstance != nullptr;
    }

    TypeId TypeInfo::GetTypeId() const
    {
        return m_type.GetTypeId();
    }

    Type TypeInfo::GetType() const
    {
        return m_type;
    }

    bool TypeInfo::IsDerivedFrom(const TypeId& typeId) const
    {
        // First check if any direct parent is of typeId. THis is in the hope that
        // we don't need to go through all out parent checking.
        for (const TypeInfo& info : m_parentTypeInfos)
        {
            if (info.GetTypeId() == typeId) 
            {
                return true;
            }
        }

        // Check if our parent is derived from typeId.
        for (const TypeInfo& info : m_parentTypeInfos)
        {
            if (info.IsDerivedFrom(typeId))
            {
                return true;
            }
        }
        return false;
    }

    std::vector<TypeInfo> TypeInfo::GetParentInfos() const
    {
        return m_parentTypeInfos;
    }

    std::vector<MemberInfo> TypeInfo::GetMemberInfos() const
    {
        std::vector<MemberInfo> members;
        std::copy(m_memberInfos.begin(), m_memberInfos.end(), std::back_inserter(members));

        for (size_t i = 0; i < m_parentTypeInfos.size(); ++i)
        {
            const TypeInfo& parentTypeInfo = m_parentTypeInfos[i];
            const std::vector<MemberInfo> parentMemberInfos = parentTypeInfo.GetMemberInfos();
            std::move(parentMemberInfos.begin(), parentMemberInfos.end(), std::back_inserter(members));
        }

        return members;
    }

    MemberInfo TypeInfo::GetMemberInfo(std::string_view memberName) const
    {
        for (const MemberInfo& info : m_memberInfos)
        {
            if (info.GetName() == memberName)
            {
                return info;
            }
        }

        for (size_t i = 0; i < m_parentTypeInfos.size(); ++i)
        {
            const TypeInfo& parentTypeInfo = m_parentTypeInfos[i];
            if (MemberInfo parentMemberInfo = parentTypeInfo.GetMemberInfo(memberName);
                parentMemberInfo.IsValid())
            {
                return parentMemberInfo;
            }
        }

        return MemberInfo();
    }

    std::vector<MemberInfo> TypeInfo::GetMemberInfosWithFlag(std::string_view flag) const
    {
        return GetMemberInfosWithFlags({ std::string(flag) });
    }

    std::vector<MemberInfo> TypeInfo::GetMemberInfosWithFlags(std::vector<std::string> flags) const
    {
        std::vector<MemberInfo> members;
        std::unordered_set<int> addedMembers;

        int index = 0;
        for (const MemberInfo& info : m_memberInfos)
        {
            for (const std::string flag : flags)
            {
                if (info.HasFlag(flag) && addedMembers.find(index) == addedMembers.end())
                {
                    members.push_back(info);
                    addedMembers.insert(index);
                }
            }
            ++index;
        }

        for (size_t i = 0; i < m_parentTypeInfos.size(); ++i)
        {
            const TypeInfo& parentTypeInfo = m_parentTypeInfos[i];
            const std::vector<MemberInfo> parentMemberInfos = parentTypeInfo.GetMemberInfosWithFlags(flags);
            std::move(parentMemberInfos.begin(), parentMemberInfos.end(), std::back_inserter(members));
        }

        return members;
    }

    FunctionInfo TypeInfo::GetFunctionInfo(std::string_view functionName) const
    {
        for (const FunctionInfo& info : m_functionInfos)
        {
            if (info.GetName() == functionName)
            {
                return info;
            }
        }

        for (size_t i = 0; i < m_parentTypeInfos.size(); ++i)
        {
            const TypeInfo& parentTypeInfo = m_parentTypeInfos[i];
            if (FunctionInfo parentFunctionInfo = parentTypeInfo.GetFunctionInfo(functionName);
                parentFunctionInfo.IsValid())
            {
                return parentFunctionInfo;
            }
        }

        return FunctionInfo();
    }

    std::vector<FunctionInfo> TypeInfo::GetFunctionInfosWithFlag(std::string_view flag) const
    {
        return GetFunctionInfosWithFlags({ std::string(flag) });
    }

    std::vector<FunctionInfo> TypeInfo::GetFunctionInfosWithFlags(std::vector<std::string> flags) const
    {
        std::vector<FunctionInfo> functions;
        std::unordered_set<int> addedFunctions;

        int index = 0;
        for (const FunctionInfo& info : m_functionInfos)
        {
            for (const std::string flag : flags)
            {
                if (info.HasFlag(flag) && addedFunctions.find(index) == addedFunctions.end())
                {
                    functions.push_back(info);
                    addedFunctions.insert(index);
                }
            }
            ++index;
        }

        for (size_t i = 0; i < m_parentTypeInfos.size(); ++i)
        {
            const TypeInfo& parentTypeInfo = m_parentTypeInfos[i];
            const std::vector<FunctionInfo> parentFunctionInfos = parentTypeInfo.GetFunctionInfosWithFlags(flags);
            std::move(parentFunctionInfos.begin(), parentFunctionInfos.end(), std::back_inserter(functions));
        }

        return functions;
    }

    std::vector<FunctionInfo> TypeInfo::GetFunctionInfos() const
    {
        std::vector<FunctionInfo> functions;
        std::copy(m_functionInfos.begin(), m_functionInfos.end(), std::back_inserter(functions));

        for (size_t i = 0; i < m_parentTypeInfos.size(); ++i)
        {
            const TypeInfo& parentTypeInfo = m_parentTypeInfos[i];
            const std::vector<FunctionInfo> parentFunctionInfos = parentTypeInfo.GetFunctionInfos();
            std::move(parentFunctionInfos.begin(), parentFunctionInfos.end(), std::back_inserter(functions));
        }

        return functions;
    }

    void TypeInfo::SetObjectInstance(void* objectInstance)
    {
        m_objectInstance = objectInstance;
        for (TypeInfo& info : m_parentTypeInfos)
        {
            info.SetObjectInstance(objectInstance);
        }

        for (MemberInfo& info : m_memberInfos)
        {
            info.SetObjectInstance(objectInstance);
        }
        for (FunctionInfo& info : m_functionInfos)
        {
            info.SetObjectInstance(objectInstance);
        }
    }
}