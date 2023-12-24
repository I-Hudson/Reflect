#pragma once

#include "Structs/Type.h"
#include "Structs/FunctionPtrArgs.h"

namespace Reflect
{
    class TypeInfo;

    /// <summary>
    /// Store arguments for a function ptr
    /// </summary>
    struct REFLECT_API FunctionInfoArgs
    {
    public:
        struct Arg
        {
            Arg(Type type, void* ptr)
                : Type(type)
                , Ptr(ptr)
            { }

            Type GetType() const { return Type; }
            void* Get() const { return Ptr; }

        private:
            Type Type;
            void* Ptr;
        };

        FunctionInfoArgs() { }
        FunctionInfoArgs(const std::vector<Arg>& args)
            : m_args(args)
        { }

        Arg GetArg(uint32_t index) const
        {
            return m_args.at(index);
        }

        template<typename T>
        void AddArg(T* obj)
        {
            m_args.push_back(Arg(Type::MakeType<T>(), obj));
        }

        void Clear() { m_args.clear(); }

        uint32_t GetSize() const { return static_cast<uint32_t>(m_args.size()); }

    private:
        std::vector<Arg> m_args;
    };

    using FunctionInfoPtr = Reflect::EReflectReturnCode(*)(void* objectPtr, void* returnValue, FunctionInfoArgs& args);

    /// @brief A FunctionInfo is a representation of a function on a TypeInfo.
    class REFLECT_API FunctionInfo
    {
    public:
        FunctionInfo();
        FunctionInfo(Type returnType, std::string_view functionName, std::vector<Type> argumentTypes, std::vector<std::string> flags, FunctionInfoPtr functionPtr, void* objectInstance);
        ~FunctionInfo();

        operator bool() const;
        bool IsValid() const;

        TypeId GetRetrunTypeId() const;
        Type GetReturnType() const;

        std::string_view GetFunctionName() const;
        const std::vector<Type>& GetArguementTypes() const;

        bool HasFlag(std::string_view flag) const;
        bool HasAnyFlags(const std::vector<std::string>& flags) const;
        const std::vector<std::string>& GetFlags() const;

        EReflectReturnCode Invoke() const;
        EReflectReturnCode Invoke(const FunctionInfoArgs& args) const;
        template<typename TReturnType>
        EReflectReturnCode Invoke(TReturnType* returnPtr, const FunctionInfoArgs& args) const
        {
            return InternaInvoke(returnPtr, args, Type::MakeType<TReturnType>());
        }

    private:
        EReflectReturnCode InternaInvoke(void* returnValue, FunctionInfoArgs functionArgs, const Type& returnType) const;

        bool VerifyObjectInstance() const;
        bool VerifyFunctionPointer() const;
        bool VerifyArgs(const FunctionInfoArgs& functionArgs) const;
        bool CheckReturnType(const Type& returnType) const;

        void SetObjectInstance(void* objectInstance);

    private:
        Type m_returnType;
        std::string m_functionName;
        std::vector<std::string> m_flags;

        /// @brief Function arguments in order.
        std::vector<Type> m_argumentTypes;

        void* m_objectInstance = nullptr;
        FunctionInfoPtr m_functionPtr = nullptr;

        friend TypeInfo;
    };
}