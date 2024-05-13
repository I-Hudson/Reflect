#include "Reflect/Structs/PropertyMeta.h"

namespace Reflect
{
    PropertyMeta::PropertyMeta(const char* key, const char* value)
        : m_key(key)
        , m_value(value)
    { }

    PropertyMeta::operator bool() const
    {
        return IsValid();
    }

    bool PropertyMeta::IsValid() const
    {
        return !m_key.empty()
            && !m_value.empty();
    }
}