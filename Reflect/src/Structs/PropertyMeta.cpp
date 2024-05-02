#include "Structs/PropertyMeta.h"

namespace Reflect
{
    PropertyMeta::PropertyMeta(const char* key, const char* value)
        : m_key(key)
        , m_value(value)
    { }
}