#include "Reflect/Structs/Type.h"

namespace Reflect
{
    Type::Type()
    {
    }

    Type::Type(std::string_view typeName, uint64_t typeSize)
        : m_typeId(TypeId(typeName))
        , m_typeSize(typeSize)
    {
        m_prettyTypeName = Util::RemoveNamespaces(std::string(typeName));
    }

    Type::~Type()
    {
    }

    bool Type::operator==(const Type& other) const
    {
        return m_typeId == other.m_typeId;
    }

    bool Type::operator!=(const Type& other) const
    {
        return !(*this == other);
    }

    Type::operator bool() const
    {
        return IsValid();
    }

    bool Type::IsValid() const
    {
        return m_typeId.IsValid();
    }

    TypeId Type::GetTypeId() const
    {
        return m_typeId;
    }

    std::string_view Type::GetTypeName() const
    {
        return m_typeId.GetTypeName();
    }

    std::string_view Type::GetPrettyTypeName() const
    {
        return m_prettyTypeName;
    }

    uint64_t Type::GetTypeSize() const
    {
        return m_typeSize;
    }
}