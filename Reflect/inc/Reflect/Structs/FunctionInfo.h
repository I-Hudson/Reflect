#pragma once

#include "Reflect/Structs/Type.h"
#include "Reflect/Structs/Property.h"
#include "Reflect/Structs/FunctionPtrArgs.h"

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
    class REFLECT_API FunctionInfo : public Property
    {
    public:
        FunctionInfo();
        FunctionInfo(Type returnType, std::string_view functionName, std::vector<Type> argumentTypes, std::vector<std::string> flags, FunctionInfoPtr functionPtr, void* objectInstance);
        FunctionInfo(Type returnType, std::string_view functionName, std::vector<Type> argumentTypes, std::vector<std::string> flags, std::vector<PropertyMeta> propertyMetas, FunctionInfoPtr functionPtr, void* objectInstance);
        ~FunctionInfo();

        virtual bool IsValid() const override;

        const std::vector<Type>& GetArguementTypes() const;

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
        /// @brief Function arguments in order.
        std::vector<Type> m_argumentTypes;
        FunctionInfoPtr m_functionPtr = nullptr;

        friend TypeInfo;
    };
}