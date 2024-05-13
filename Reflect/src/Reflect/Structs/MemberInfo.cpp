#include "Reflect/Structs/MemberInfo.h"

#include <unordered_set>

namespace Reflect
{
    MemberInfo::MemberInfo()
    { }

    MemberInfo::MemberInfo(Type type, std::string memberName, EReflectValueType valueType, 
        EReflectValueModifier modiferType, std::vector<std::string> flags, uint64_t offset, 
        void* objectInstance)
        : Property(std::move(type), std::move(memberName), PropertyType::Member, std::move(flags), {}, objectInstance)
        , m_valueType(valueType)
        , m_modifierType(modiferType)
        , m_memberOffset(offset)
    { }

    MemberInfo::MemberInfo(Type type, std::string memberName, EReflectValueType valueType, 
        EReflectValueModifier modiferType, std::vector<std::string> flags, std::vector<PropertyMeta> propertyMetas, 
        uint64_t offset, void* objectInstance)
        : Property(std::move(type), std::move(memberName), PropertyType::Member, std::move(flags), std::move(propertyMetas), objectInstance)
        , m_valueType(valueType)
        , m_modifierType(modiferType)
        , m_memberOffset(offset)
    {
    }

    MemberInfo::~MemberInfo()
    {
    }

    EReflectValueType MemberInfo::GetValueType() const
    {
        return m_valueType;
    }

    EReflectValueModifier MemberInfo::GetModifierType() const
    {
        return m_modifierType;
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