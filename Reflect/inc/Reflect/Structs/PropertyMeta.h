#pragma once

#include "Reflect/Core/Defines.h"

#include <string>
#include <typeinfo>

namespace Reflect
{
    /// @brief Store additional meta data for a property defined within an 'REFLECT_PROPERTY'
    /// Example: REFLECT_PROPERTY(Meta(ClampMin(0.0f), ClampMax(1.0f), DebugName("Clamped Float"))
    class REFLECT_API PropertyMeta
    {
    public:
        PropertyMeta(const char* key, const char* value);

        operator bool() const;
        bool IsValid() const;

        const char* GetKey() const
        {
            return m_key.c_str();
        }

        const char* GetValue() const
        {
            return m_value.c_str();
        }

        template<typename T
        , typename = std::enable_if_t<
            std::is_same_v<int, T> 
            || std::is_same_v<float, T>
            || std::is_same_v<std::string, T>>>
        T GetValue() const
        {
            if constexpr (std::is_same_v<int, T>)
            {
                return std::stoi(m_value);
            }
            else if constexpr (std::is_same_v<float, T>)
            {
                return std::stof(m_value);
            }
            else if constexpr (std::is_same_v<std::string, T>)
            {
                return std::string(m_value);
            }
            assert(false && "[PropertyMeta::GetValue<T>] Template T must be of a valid type.");
            return T();
        }

    private:
        std::string m_key;
        std::string m_value;
    };
}

