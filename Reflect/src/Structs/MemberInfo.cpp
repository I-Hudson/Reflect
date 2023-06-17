#include "Structs/MemberInfo.h"

namespace Reflect
{
    MemberInfo::MemberInfo()
    { }

    MemberInfo::MemberInfo(Type type, EReflectValueType valueType, EReflectValueModifier modiferType, uint64_t offset, void* objectInstance)
        : m_type(std::move(type))
        , m_valueType(valueType)
        , m_modifierType(modiferType)
        , m_memberOffset(offset)
        , m_objectInstance(objectInstance)
    { }

    MemberInfo::~MemberInfo()
    {
    }

    const TypeId& MemberInfo::GetTypeId() const
    {
        return m_type.GetTypeId();
    }

    const Type& MemberInfo::GetType() const
    {
        return m_type;
    }

    EReflectValueType MemberInfo::GetValueType() const
    {
        return m_valueType;
    }

    EReflectValueModifier MemberInfo::GetModifierType() const
    {
        return m_modifierType;
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