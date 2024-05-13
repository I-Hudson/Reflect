#pragma once

#include "Reflect/Structs/MemberInfo.h"
#include "Reflect/Structs/FunctionInfo.h"

namespace Reflect
{
    /// @brief 
    class REFLECT_API TypeInfo
    {
    public:
        TypeInfo();
        TypeInfo(Type type, void* objectInstance, std::vector<TypeInfo> parentInfos, std::vector<MemberInfo> memberInfos, std::vector<FunctionInfo> functionInfos);
        ~TypeInfo();

        operator bool() const;
        bool IsValid() const;

        TypeId GetTypeId() const;
        Type GetType() const;

        bool IsDerivedFrom(const TypeId& typeId) const;
        template<typename T>
        bool IsDerivedFrom() const
        {
            TypeId typeId = TypeId::MakeTypeId<T>();
            return IsDerivedFrom(typeId);
        }

        std::vector<TypeInfo> GetParentInfos() const;

        MemberInfo GetMemberInfo(std::string_view memberName) const;
        std::vector<MemberInfo> GetMemberInfosWithFlag(std::string_view flag) const;
        std::vector<MemberInfo> GetMemberInfosWithFlags(std::vector<std::string> flags) const;
        std::vector<MemberInfo> GetMemberInfos() const;

        FunctionInfo GetFunctionInfo(std::string_view memberName) const;
        std::vector<FunctionInfo> GetFunctionInfosWithFlag(std::string_view flag) const;
        std::vector<FunctionInfo> GetFunctionInfosWithFlags(std::vector<std::string> flags) const;
        std::vector<FunctionInfo> GetFunctionInfos() const;

    private:
        void SetObjectInstance(void* objectInstance);

    private:
        Type m_type;
        void* m_objectInstance = nullptr;

        std::vector<TypeInfo> m_parentTypeInfos;
        std::vector<MemberInfo> m_memberInfos;
        std::vector<FunctionInfo> m_functionInfos;

        template<typename>
        friend class GenerateTypeInfoForType;
    };
}