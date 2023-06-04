#pragma once

#include "Core/Defines.h"
#include "Core/Enums.h"
#include "Core/Util.h"
#include <vector>
#include <memory>

struct ReflectFunction;
struct ReflectMember;

namespace Reflect
{
	struct ReflectType
	{
		bool operator!=(const ReflectType& other)
		{
			return m_typeName != other.m_typeName ||
				m_typeSize != other.m_typeSize;
		}
		bool operator==(const ReflectType& other)
		{
			return !(*this != other);
		}

		std::string GetTypeName() const { return m_typeName; }
		std::size_t GetTypeSize() const { return m_typeSize; }

		std::string GetValueTypeName() const { return m_valueTypeName; }
		std::size_t GetValueTypeSize() const { return m_valueTypeSize; }

		EReflectValueType GetValueType() const { return m_valueType; }

		std::vector<ReflectType*> GetInheritances() const;

		bool IsClass() const;
		bool IsStruct() const;
		bool IsMember() const;
		bool IsFunction() const;
		bool IsParameter() const;

		virtual void ClearValue(void* data) const = 0;
		virtual void Copy(void* src, void* dst) const = 0;
		virtual void Copy_s(void* src, void* dst, size_t dst_size) const = 0;

	protected:
		std::string m_typeName;
		std::size_t m_typeSize;

		// Store the value type (ex. int* would be int).
		std::string m_valueTypeName;
		std::size_t m_valueTypeSize;

		EReflectValueType m_typeValue = EReflectValueType::Unknown;
		EReflectValueModifier m_modifier = EReflectValueModifier::None;

		EReflectType m_eReflectType;
		EReflectValueType m_valueType = EReflectValueType::Unknown;

		std::vector<std::unique_ptr<ReflectType>> m_inheritanceTypes;
	};

	template<typename Type>
	struct ReflectTypeCPP : public ReflectType
	{
		using value_type = std::remove_pointer_t<std::remove_reference_t<Type>>;

		ReflectTypeCPP(EReflectType eType, EReflectValueType valueType, std::vector<std::unique_ptr<ReflectType>> inheritance, std::string givenName = "")
		{
			m_typeName = Util::GetTypeName<Type>();
			m_typeSize = Util::GetTypeSize<Type>();

			m_valueTypeName = Util::GetValueTypeName<Type>();
			m_valueTypeSize = Util::GetValueTypeSize<Type>();

			m_eReflectType = eType;
			m_valueType = valueType;

			m_inheritanceTypes = std::move(inheritance);
		}

		virtual void ClearValue(void* data) const override
		{
			if constexpr (!std::is_default_constructible_v<value_type>)
			{
				memset(data, 0, sizeof(value_type));
			}
			else
			{
				Type* t = static_cast<Type*>(data);
				*t = Type();
			}
		}

		virtual void Copy(void* src, void* dst) const override
		{
			if (std::is_pointer_v<Type>)
			{
				value_type* from = static_cast<value_type*>(src);
				value_type* to = static_cast<value_type*>(dst);
				// TODO: Not perfect, but should do for now.
				memcpy(to, from, sizeof(value_type*));
			}
			else
			{
				Type* from = static_cast<Type*>(src);
				Type* to = static_cast<Type*>(dst);
				*to = *from;
			}
		}

		/// <summary>
		/// Safe call for copy. Check the 'value_type' size against dst_size.
		/// </summary>
		/// <param name="src"></param>
		/// <param name="dst"></param>
		/// <param name="dst_size"></param>
		virtual void Copy_s(void* src, void* dst, size_t dst_size) const override
		{
			if (!src || !dst)
			{
				std::cerr << "[ReflectTypeCPP::Copy_s] src: '" << src << "' or dst: '" << dst << "' are not valid.";
			}
			else if (sizeof(value_type) != dst_size)
			{
				std::cerr << "[ReflectTypeCPP::Copy_s] dst_size: '" << dst_size << "' does not match value_type size : '" << sizeof(value_type) << "'.";
			}
			else
			{
				Copy(src, dst);
			}
		}

	};

	template<>
	struct ReflectTypeCPP<void> : public ReflectType
	{
		using value_type = std::remove_pointer_t<std::remove_reference_t<void>>;

		ReflectTypeCPP(EReflectType eType, EReflectValueType valueType, std::vector<std::unique_ptr<ReflectType>> inheritance, std::string givenName = "")
		{
			m_typeName = "void";
			m_typeSize = 0;

			m_valueTypeName = "void";
			m_valueTypeSize = 0;

			m_eReflectType = eType;
			m_valueType = EReflectValueType::Unknown;

			m_inheritanceTypes = std::move(inheritance);
		}

		virtual void ClearValue(void* data) const override
		{ }

		virtual void Copy(void* src, void* dst) const override
		{ }

		/// <summary>
		/// Safe call for copy. Check the 'value_type' size against dst_size.
		/// </summary>
		/// <param name="src"></param>
		/// <param name="dst"></param>
		/// <param name="dst_size"></param>
		virtual void Copy_s(void* src, void* dst, size_t dst_size) const override
		{ }
	};

	/// <summary>
/// Store arguments for a function ptr
/// </summary>
	struct FunctionPtrArgs
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

	using FunctionPtr = Reflect::EReflectReturnCode(*)(void* objectPtr, void* returnValue, FunctionPtrArgs& args);


#ifdef REFLECT_TYPE_INFO_ENABLED
	template<typename T>
	class GenerateTypeInfoForType;

	class REFLECT_API ReflectTypeInfo;

	/// @brief Describe a single member variable within a class/struct.
	class REFLECT_API ReflectTypeMember
	{
	public:
		ReflectTypeMember(void* ownerClass, const char* name, void* memberPtr, std::unique_ptr<ReflectType> type,
			std::vector<std::string> flags);

		ReflectType* GetType() const;
		std::string_view GetName() const;
		bool IsValid() const;
		bool HasFlag(const char* flag) const;
		void* GetData() const;

		template<typename T>
		T* ConvertToType()
		{
			std::string convertType = Reflect::Util::GetTypeName<T>();
			if (!IsValid() || convertType != m_type->GetTypeName())
			{
				return nullptr;
			}
			return static_cast<T*>(m_memberPtr);
		}

		template<typename T>
		void ModifyValue(T value)
		{
			T* ptr = ConvertToType<T>();
			if (!ptr)
			{
				return;
			}
			*ptr = value;
		}

	private:
		void* m_ownerClass = nullptr;
		void* m_memberPtr = nullptr;
		std::string m_name;
		std::unique_ptr<ReflectType> m_type;
		std::vector<std::string> m_flags;

		friend class ReflectTypeInfo;
	};

	/// @brief Describe a a single function within a class/struct.
	class REFLECT_API ReflectTypeFunction
	{
	public:
		ReflectTypeFunction(void* ownerClass, const char* name, FunctionPtr funcPtr
			, std::unique_ptr<ReflectType> info, std::vector<std::unique_ptr<ReflectType>> args);

		Reflect::EReflectReturnCode Invoke(FunctionPtrArgs functionArgs = FunctionPtrArgs());
		//TODO: FunctionPtr returnValue needs to be a pointer to a pointer. This will allow pointers to be returned from functions.
		template<typename T>
		Reflect::EReflectReturnCode Invoke(T* returnValue, FunctionPtrArgs functionArgs = FunctionPtrArgs())
		{
			return CallInternal((void*)returnValue
				, std::move(functionArgs)
				, ReflectTypeCPP<T>(EReflectType::Unknown, EReflectValueType::Unknown, std::vector<std::unique_ptr<ReflectType>>()));
		}

		bool IsValid() const;

		ReflectType* GetInfo() const;
		std::string_view GetName() const;
		const ReflectType* GetArgInfo(int index) const;
		std::vector<ReflectType*> GetArgsInfo() const;

	protected:
		EReflectReturnCode CallInternal(void* returnValue, FunctionPtrArgs functionArgs, const ReflectType& returnType);

	private:
		bool VerifyOwnerObject() const;
		bool VerifyFunctionPointer() const;
		bool VerifyArgs(const FunctionPtrArgs& functionArgs) const;
		bool CheckReturnType(const ReflectType& returnType) const;

	protected:
		std::unique_ptr<ReflectType> m_info;
		std::string m_name;

		void* m_ownerClass = nullptr;
		FunctionPtr m_func = nullptr;

		int m_numOfArgs;
		std::vector<std::unique_ptr<ReflectType>> m_argsInfo;

		friend class ReflectTypeInfo;
	};

	/// @brief Describe from a high level the class/struct. This should contain basic info such as object name, size, 
	class REFLECT_API ReflectTypeInfo
	{
		using ConstructFunc = void* (*)();

	public:
		ReflectTypeInfo(void* owner_class, std::unique_ptr<ReflectType> info
			, std::vector<std::unique_ptr<ReflectTypeInfo>> inheritances
			, std::vector<std::unique_ptr<ReflectTypeMember>> members
			, std::vector<std::unique_ptr<ReflectTypeFunction>> functions);

		ReflectTypeInfo(const ReflectTypeInfo& other) = delete;
		ReflectTypeInfo(ReflectTypeInfo&& other) noexcept
		{
			(*this) = std::move(other);
		}

		ReflectTypeInfo& operator=(const ReflectTypeInfo& other) = delete;
		ReflectTypeInfo& operator=(ReflectTypeInfo&& other) noexcept
		{
			m_owner_class = std::move(other.m_owner_class);
			m_construct_func = std::move(other.m_construct_func);
			m_info = std::move(other.m_info);
			m_inheritances = std::move(other.m_inheritances);
			m_members = std::move(other.m_members);
			m_functions = std::move(other.m_functions);

			other.m_owner_class = nullptr;
			other.m_construct_func = nullptr;
			other.m_info.reset();
			other.m_inheritances.clear();
			other.m_members.clear();
			other.m_functions.clear();

			return *this;
		}

		/// @brief Return type info.
		/// @return ReflectType*
		ReflectType* GetInfo() const;

		/// @brief Return weather or not this instance of 'ReflectTypeInfo' has an
		/// owner class/struct tied to it.
		/// @return bool
		bool HasOwner() const;

		/// @brief Construct a new object. Creates a heap allocated object with 'new'. Reflect does not manage the lifetime of this object.
		/// @return void*.
		void* ConstructNew() { if (m_construct_func) { return m_construct_func(); } return nullptr; }

		/// @brief Return a ReflectTypeMember* if found.
		/// @return ReflectTypeMember*
		ReflectTypeMember* GetMember(const char* memberName) const;
		ReflectTypeMember* GetMember(const char* memberName, bool includeBaseClasses) const;

		/// @brief Return all members.
		/// @return std::vector<ReflectTypeMember*>.
		std::vector<ReflectTypeMember*> GetAllMembers() const;
		std::vector<ReflectTypeMember*> GetAllMembers(bool includeBaseClasses) const;

		/// @brief Return all members with flags given.
		/// @return std::vector<ReflectTypeMember*>.
		std::vector<ReflectTypeMember*> GetAllMembersWithFlags(std::vector<const char*> flags) const;
		std::vector<ReflectTypeMember*> GetAllMembersWithFlags(std::vector<const char*> flags, bool includeBaseClasses) const;

		/// @brief Return a ReflectTypeFunction* if found.
		/// @return ReflectTypeFunction*.
		ReflectTypeFunction* GetFunction(const char* functionName) const;
		ReflectTypeFunction* GetFunction(const char* functionName, bool includeBaseClasses) const;

	private:
		/// @breif Pointer to an instance of which this TypeInfo is made from. This will be nullptr if no instance pointer is given.
		void* m_owner_class = nullptr;

		/// @brief Construct function.
		ConstructFunc m_construct_func = nullptr;

		/// @brief Base info of this class/struct type.
		std::unique_ptr<ReflectType> m_info;
		/// @brief All class/structs this class/struct inheritances from.
		std::vector<std::unique_ptr<ReflectTypeInfo>> m_inheritances;
		/// @brief Store all the members for this type.
		std::vector<std::unique_ptr<ReflectTypeMember>> m_members;
		/// @brief Store all the functions for this type.
		std::vector<std::unique_ptr<ReflectTypeFunction>> m_functions;

		template<typename>
		friend class GenerateTypeInfoForType;
	};

	/// @brief Template for generating a type's info. Must have a specialisation for each type.
	template<typename T>
	class GenerateTypeInfoForType
	{
	public:
		ReflectTypeInfo GetTypeInfo(T* ownerClass)
		{
			assert(false && "[GenerateTypeInfoForType] This must have a template specialisation.");
			return ReflectTypeInfo(ownerClass, std::make_unique<ReflectTypeCPP<T>>(), {});
		}
	};

	template<class T>
	struct movable_il {
		mutable T t;
		operator T() const&& { return std::move(t); }
		movable_il(T&& in) : t(std::move(in)) {}
	};

	template<class T, class A = std::allocator<T>>
	std::vector<T, A> vector_from_il(std::initializer_list< movable_il<T> > il) {
		std::vector<T, A> r(std::make_move_iterator(il.begin()), std::make_move_iterator(il.end()));
		return r;
	}

#else 
	struct ReflectMemberProp
	{
		ReflectMemberProp(const char* name, ReflectType* typeCPP, size_t offset, std::vector<std::string> const& strProperties)
			: Name(name)
			, Type(typeCPP)
			, Offset(offset)
			, StrProperties(strProperties)
		{ }

		~ReflectMemberProp()
		{
			delete Type;
		}

		bool ContainsProperty(std::vector<std::string> const& flags)
		{
			for (auto const& flag : flags)
			{
				for (auto const& p : StrProperties)
				{
					if (p == flag)
					{
						return true;
					}
				}
			}
			return false;
		}

		const char* Name;
		ReflectType* Type;
		size_t Offset;
		std::vector<std::string> StrProperties;
	};

	struct ReflectFunction
	{
		ReflectFunction(void* objectPtr, FunctionPtr func)
			: m_objectPtr(objectPtr)
			, m_func(func)
		{ }

		//template<typename... Args>
		//void Invoke(void* returnValue, Args... args)
		//{
		//	FunctionPtrArgs funcArgs = PackFunctionArgs(std::forward<Args>(args)...);
		//	int i = *static_cast<int*>(funcArgs.GetArg(0));
		//	int* ip = static_cast<int*>(funcArgs.GetArg(1));
		//	return (*Func)(ObjectPtr, returnValue, funcArgs);
		//}

		//TODO: FunctionPtr returnValuePointer needs to be a pointer to a pointer. This will allow pointers to be returned from functions.
		template<typename T>
		Reflect::EReflectReturnCode Invoke(T* returnValue, FunctionPtrArgs functionArgs = FunctionPtrArgs())
		{
			return Invoke((void*)returnValue, std::move(functionArgs));
		}

		Reflect::EReflectReturnCode Invoke(void* returnValue = nullptr, FunctionPtrArgs functionArgs = FunctionPtrArgs())
		{
			if (IsValid())
			{
				(*m_func)(m_objectPtr, returnValue, functionArgs);
				return EReflectReturnCode::SUCCESS;
			}
			return EReflectReturnCode::INVALID_FUNCTION_POINTER;
		}

		bool IsValid() const
		{
			return m_objectPtr != nullptr;
		}

	private:
		template<typename... Args>
		FunctionPtrArgs PackFunctionArgs(Args... args)
		{
			std::vector<FunctionPtrArgs::Arg> funcArgs = { PackFunctionArg(args)... };
			return  FunctionPtrArgs(funcArgs);
		}

		template<typename T, typename... Args>
		FunctionPtrArgs::Arg PackFunctionArg(T& t, Args&... args)
		{
			return FunctionPtrArgs::Arg(Reflect::Util::GetTypeName(t), &t);
		}

		template<typename T, typename... Args>
		FunctionPtrArgs::Arg PackFunctionArg(T* t, Args... args)
		{
			return FunctionPtrArgs::Arg(Reflect::Util::GetTypeName(t), static_cast<void*>(t));
		}

	private:
		void* m_objectPtr;
		FunctionPtr m_func;
	};

	struct ReflectMember
	{
		ReflectMember(const char* memberName, ReflectType* type, void* memberPtr)
			: m_name(memberName)
			, m_type(type)
			, m_ptr(memberPtr)
		{}

		bool IsValid() const
		{
			return m_ptr != nullptr;
		}

		void* GetRawPointer() { return m_ptr; }

		std::string GetName() { return m_name; }

		const ReflectType* GetType() const { return m_type; }

		template<typename T>
		T* ConvertToType()
		{
			std::string convertType = Reflect::Util::GetTypeName<T>();
			if (!IsValid() || convertType != m_type->GetTypeName())
			{
				return nullptr;
			}
			return static_cast<T*>(m_ptr);
		}

	private:
		const char* m_name;
		ReflectType* m_type;
		void* m_ptr;
		int m_offset;
	};
#endif

	struct REFLECT_API IReflect
	{
#ifndef REFLECT_TYPE_INFO_ENABLED
		virtual ReflectFunction GetFunction(const char* functionName) { (void)functionName; return ReflectFunction(nullptr, nullptr); };
		virtual ReflectMember GetMember(const char* memberName) { (void)memberName; return ReflectMember("", nullptr, nullptr); };
		virtual std::vector<ReflectMember> GetMembers(std::vector<std::string> const& flags) { (void)flags; return {}; };
		virtual std::vector<ReflectMember> GetAllMembers() { return {}; };
#endif
	};
}

#define REFLECT_BASE public Reflect::IReflect
#define REFLECT_STRUCT(...)
#define REFLECT_CLASS(...)
