#pragma once

#include "Structs/Type.h"

namespace Reflect
{
    /// @brief A MemberInfo is a representation of a member variable which has been reflected 
    /// on a class/struct.
    class MemberInfo
    {
    public:
        MemberInfo();
        MemberInfo(Type type, EReflectValueType valueType, EReflectValueModifier modiferType, uint64_t offset);
        MemberInfo(Type type, EReflectValueType valueType, EReflectValueModifier modiferType, uint64_t offset, void* objectInstance);
        ~MemberInfo();

        const TypeId& GetTypeId() const;
        const Type& GetType() const;

        EReflectValueType GetValueType() const;
        EReflectValueModifier GetModifierType() const;

        void* GetMemberPointer() const;
        void* GetMemberPointer(void* objectInstance) const;

    private:
        /// @brief The type of the member.
        Type m_type;

        /// @brief Is this member of type value, pointer or reference.
        EReflectValueType m_valueType = EReflectValueType::Unknown;
        /// @brief Is this member a const, static or volatile.
        EReflectValueModifier m_modifierType = EReflectValueModifier::None;

        /// @brief Offset of this member in the object.
        uint64_t m_memberOffset = 0;
        /// @brief The object instance, can be null and given later.
        void* m_objectInstance = nullptr;
    };
}