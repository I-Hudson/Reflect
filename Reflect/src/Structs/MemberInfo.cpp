#include "Structs/MemberInfo.h"

namespace Reflect
{

    MemberInfo::MemberInfo()
    { }

    MemberInfo::MemberInfo(Type type, EReflectValueType valueType, EReflectValueModifier modiferType, uint64_t offset)
        : MemberInfo(std::move(type)
        , valueType
        , modiferType
        ,  offset
        , nullptr)
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
        return GetMemberPointer(m_objectInstance);
    }

    void* MemberInfo::GetMemberPointer(void* objectInstance) const
    {
        if (!objectInstance)
        {
            return nullptr;
        }

        void* memberPtr = (unsigned char*)objectInstance + m_memberOffset;
        return memberPtr;
    }

}