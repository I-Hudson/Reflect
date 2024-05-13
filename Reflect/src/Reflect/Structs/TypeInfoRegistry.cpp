#include "Reflect/Structs/TypeInfoRegistry.h"

namespace Reflect
{
    TypeInfoRegistry::TypeInfoRegistry()
    {
    }

    TypeInfoRegistry::~TypeInfoRegistry()
    {
    }

    void TypeInfoRegistry::RegisterTypeInfo(TypeId typeId, CreateTypeInfoFunc createTypeInfoFunc)
    {
        std::lock_guard lock(Instance().m_registryLock);

        if (auto iter = Instance().m_registry.find(typeId);
            iter != Instance().m_registry.end())
        {
            return;
        }
        Instance().m_registry[typeId] = createTypeInfoFunc;
    }

    void TypeInfoRegistry::UnregisterTypeInfo(TypeId typeId)
    {
        std::lock_guard lock(Instance().m_registryLock);

        if (auto iter = Instance().m_registry.find(typeId);
            iter != Instance().m_registry.end())
        {
            Instance().m_registry.erase(typeId);
        }
    }

    bool TypeInfoRegistry::HasTypeInfo(const TypeId& typeId) const
    {
        std::lock_guard typeInfosLock(Instance().m_registryLock);
        for (const auto& [typeTypeId, createFunc] : Instance().m_registry)
        {
            if (typeId == typeTypeId)
            {
                return true;
            }
        }
        return false;
    }

    TypeInfo TypeInfoRegistry::GetTypeInfo(const TypeId& typeId)
    {
        return GetTypeInfo(typeId, nullptr);
    }

    TypeInfo TypeInfoRegistry::GetTypeInfo(const TypeId& typeId, void* objectInstance)
    {
        std::lock_guard typeInfosLock(Instance().m_registryLock);
        for (const auto& [typeTypeId, createFunc] : Instance().m_registry)
        {
            if (typeId == typeTypeId)
            {
                return createFunc(objectInstance);
            }
        }
        return TypeInfo();
    }
}