#pragma once

#include "Structs/Type.h"
#include "Structs/FunctionPtrArgs.h"

namespace Reflect
{
    using FunctionInfoPtr = Reflect::EReflectReturnCode(*)(void* objectPtr, void* returnValue, FunctionPtrArgs& args);

    /// @brief A FunctionInfo is a representation of a function on a TypeInfo.
    class REFLECT_API FunctionInfo
    {
    public:
        FunctionInfo();
        FunctionInfo(Type returnType, std::string_view functionName, std::vector<Type> argumentTypes, FunctionInfoPtr functionPtr, void* objectInstance);
        ~FunctionInfo();

        const TypeId& GetRetrunTypeId() const;
        const Type& GetReturnType() const;

        std::string_view GetFunctionName() const;
        const std::vector<Type>& GetArguementTypes() const;

        EReflectReturnCode Invoke() const;
        EReflectReturnCode Invoke(const FunctionPtrArgs& args) const;
        template<typename TReturnType>
        EReflectReturnCode Invoke(TReturnType* returnPtr, const FunctionPtrArgs& args) const
        {
            return InternaInvoke(returnPtr, args, Type::MakeType<TReturnType>());
        }

    private:
        EReflectReturnCode InternaInvoke(void* returnValue, FunctionPtrArgs functionArgs, const Type& returnType) const;

        bool VerifyObjectInstance() const;
        bool VerifyFunctionPointer() const;
        bool VerifyArgs(const FunctionPtrArgs& functionArgs) const;
        bool CheckReturnType(const Type& returnType) const;

    private:
        Type m_returnType;
        std::string m_functionName;

        /// @brief Function arguments in order.
        std::vector<Type> m_argumentTypes;

        void* m_objectInstance = nullptr;
        FunctionInfoPtr m_functionPtr = nullptr;
    };
}