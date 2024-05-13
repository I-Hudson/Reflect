#pragma once

#include "Reflect/Core/Defines.h"
#include "Reflect/Core/Util.h"

#include <string>
#include <vector>

namespace Reflect
{
    /// <summary>
    /// Store arguments for a function ptr
    /// </summary>
    struct REFLECT_API FunctionPtrArgs
    {
    public:
        struct Arg
        {
            Arg(std::string type, void* ptr)
                : Type(type)
                , Ptr(ptr)
            { }

            std::string GetType() const { return Type; }
            void* Get() const { return Ptr; }

        private:
            std::string Type;
            void* Ptr;
        };

        FunctionPtrArgs() { }
        FunctionPtrArgs(const std::vector<Arg>& args)
            : m_args(args)
        { }

        Arg GetArg(uint32_t index) const
        {
            return m_args.at(index);
        }

        template<typename T>
        void AddArg(T* obj)
        {
            m_args.push_back(Arg(Reflect::Util::GetTypeName(*obj), obj));
        }

        void Clear() { m_args.clear(); }

        uint32_t GetSize() const { return static_cast<uint32_t>(m_args.size()); }

    private:
        std::vector<Arg> m_args;
    };
}