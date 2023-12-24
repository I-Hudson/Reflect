#pragma once

#include "Structs/Type.h"

namespace Reflect
{
    class TypeInfo;

    /// @brief A MemberInfo is a representation of a member variable which has been reflected 
    /// on a class/struct.
    class REFLECT_API MemberInfo
    {
    public:
        MemberInfo();
        MemberInfo(Type type, std::string memberName, EReflectValueType valueType, EReflectValueModifier modiferType, std::vector<std::string> flags, uint64_t offset, void* objectInstance);
        ~MemberInfo();

        operator bool() const;
        bool IsValid() const;

        TypeId GetTypeId() const;
        Type GetType() const;

        std::string_view GetMemberName() const;

        EReflectValueType GetValueType() const;
        EReflectValueModifier GetModifierType() const;

        bool HasFlag(std::string_view flag) const;
        bool HasAnyFlags(const std::vector<std::string>& flags) const;
        const std::vector<std::string>& GetFlags() const;

        void* GetMemberPointer() const;
        template<typename T>
        T* GetMemberPointer() const
        {
            void* memberPtr = GetMemberPointer();
            if (!memberPtr)
            {
                return nullptr;
            }
            return static_cast<T*>(memberPtr);
        }

    private:
        void SetObjectInstance(void* objectInstance);

    private:
        /// @brief The type of the member.
        Type m_type;
        std::string m_memberName;

        /// @brief Is this member of type value, pointer or reference.
        EReflectValueType m_valueType = EReflectValueType::Unknown;
        /// @brief Is this member a const, static or volatile.
        EReflectValueModifier m_modifierType = EReflectValueModifier::None;

        std::vector<std::string> m_flags;

        /// @brief Offset of this member in the object.
        uint64_t m_memberOffset = 0;
        /// @brief The object instance, can be null and given later.
        void* m_objectInstance = nullptr;

        friend TypeInfo;
    };
}