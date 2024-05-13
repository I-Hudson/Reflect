#pragma once

#include "Reflect/Structs/Type.h"
#include "Reflect/Structs/PropertyMeta.h"

#include <vector>

namespace Reflect
{
    enum class PropertyType
    {
        Member,
        Function
    };

    class REFLECT_API Property
    {
    public:
        Property();
        Property(Type objectType, std::string name, PropertyType propertyType, std::vector<std::string> flags, std::vector<PropertyMeta> propertyMetas, void* objectInstance);

        operator bool() const;
        virtual bool IsValid() const;

        Type GetType() const;
        TypeId GetTypeId() const;
        std::string_view GetName() const;

        bool HasFlag(std::string_view flag) const;
        bool HasAnyFlags(const std::vector<std::string>& flags) const;
        const std::vector<std::string>& GetFlags() const;

        PropertyType GetPropertyType() const;
        PropertyMeta GetMeta(std::string_view propertyName) const;
        const std::vector<PropertyMeta>& GetAllMetas() const;

    protected:
        /// @brief Define the member type or the function return type.
        Type m_objectType;
        std::string m_name;
        /// @brief The object instance, can be null and given later.
        void* m_objectInstance = nullptr;

        std::vector<std::string> m_flags;
        std::vector<PropertyMeta> m_propertyMetas;
        PropertyType m_propertyType;
    };
}