#pragma once

#include "Reflect/Structs/TypeInfo.h"

#include <unordered_map>
#include <mutex>

namespace Reflect
{
    class REFLECT_API TypeInfoRegistry
    {
    public:
        using CreateTypeInfoFunc = TypeInfo(*)(void* objectInstance);

        TypeInfoRegistry();
        ~TypeInfoRegistry();

        static TypeInfoRegistry& Instance()
        {
            static TypeInfoRegistry instance;
            return instance;
        }

        static void RegisterTypeInfo(TypeId typeId, CreateTypeInfoFunc createTypeInfoFunc);
        static void UnregisterTypeInfo(TypeId typeId);

        bool HasTypeInfo(const TypeId& typeId) const;

        static TypeInfo GetTypeInfo(const TypeId& typeId);
        static TypeInfo GetTypeInfo(const TypeId& typeId, void* objectInstance);
        template<typename T>
        static TypeInfo GetTypeInfo()
        {
            return GetTypeInfo<T>(nullptr);
        }
        template<typename T>
        static TypeInfo GetTypeInfo(void* objectInstance)
        {
            static_assert(std::is_base_of_v<IReflect, T>);
            return GetTypeInfo(T::GetStaticTypeInfo().GetTypeId(), objectInstance);
        }

    private:
        std::unordered_map<TypeId, CreateTypeInfoFunc> m_registry;
        std::mutex m_registryLock;
    };

    /// @brief Allow for a type to be registered when the constructor is called,
    /// and unregistered when the destructor is called.
    struct REFLECT_API TypeInfoRegister
    {
        TypeId m_typeId;
        TypeInfoRegister() { }

        TypeInfoRegister(const char* typeName, TypeInfoRegistry::CreateTypeInfoFunc createTypeInfoFunc)
        {
            m_typeId = TypeId(typeName);
            TypeInfoRegistry::RegisterTypeInfo(m_typeId, createTypeInfoFunc);
        }

        ~TypeInfoRegister()
        {
            TypeInfoRegistry::UnregisterTypeInfo(m_typeId);
        }
    };
}