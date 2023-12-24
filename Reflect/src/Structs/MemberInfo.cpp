#include "Structs/MemberInfo.h"

#include <unordered_set>

namespace Reflect
{
    MemberInfo::MemberInfo()
    { }

    MemberInfo::MemberInfo(Type type, std::string memberName, EReflectValueType valueType, EReflectValueModifier modiferType, std::vector<std::string> flags, uint64_t offset, void* objectInstance)
        : m_type(std::move(type))
        , m_memberName(std::move(memberName))
        , m_valueType(valueType)
        , m_modifierType(modiferType)
        , m_flags(std::move(flags))
        , m_memberOffset(offset)
        , m_objectInstance(objectInstance)
    { }

    MemberInfo::~MemberInfo()
    {
    }

    MemberInfo::operator bool() const
    {
        return IsValid();
    }

    bool MemberInfo::IsValid() const
    {
        return m_type.IsValid()
            && !m_memberName.empty()
            && m_objectInstance != nullptr;
    }

    TypeId MemberInfo::GetTypeId() const
    {
        return m_type.GetTypeId();
    }

    Type MemberInfo::GetType() const
    {
        return m_type;
    }

    std::string_view MemberInfo::GetMemberName() const
    {
        return m_memberName;
    }

    EReflectValueType MemberInfo::GetValueType() const
    {
        return m_valueType;
    }

    EReflectValueModifier MemberInfo::GetModifierType() const
    {
        return m_modifierType;
    }

    bool MemberInfo::HasFlag(std::string_view flag) const
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

    bool MemberInfo::HasAnyFlags(const std::vector<std::string>& flags) const
    {
        std::unordered_set<std::string_view> flagsSet;
        for (const std::string& memberFlag : m_flags)
        {
            flagsSet.insert(memberFlag);
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

    const std::vector<std::string>& MemberInfo::GetFlags() const
    {
        return m_flags;
    }

    void MemberInfo::SetObjectInstance(void* objectInstance)
    {
        m_objectInstance = objectInstance;
    }

    void* MemberInfo::GetMemberPointer() const
    {
        if (!m_objectInstance)
        {
            return nullptr;
        }

        unsigned char* objectInstance = (unsigned char*)m_objectInstance;
        void* memberPtr = objectInstance + m_memberOffset;
        return memberPtr;
    }
}