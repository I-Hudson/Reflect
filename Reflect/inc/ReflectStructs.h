#pragma once

#include "Core/Core.h"
#include "Core/Enums.h"
#include "Core/Util.h"
#include <vector>
#include <functional>
#include <type_traits>
#include <unordered_map>

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

		std::string GetGivenName() const { return m_givenName; }

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

		std::string m_givenName;
		EReflectType m_eReflectType;
	};

	template<typename Type>
	struct ReflectTypeCPP : public ReflectType
	{
		using value_type = std::remove_pointer_t<std::remove_reference_t<Type>>;
	
		ReflectTypeCPP(EReflectType eType, std::string givenName = "")
		{
			m_typeName = Util::GetTypeName<Type>();
			m_typeSize = Util::GetTypeSize<Type>();

			m_valueTypeName = Util::GetValueTypeName<Type>();
			m_valueTypeSize = Util::GetValueTypeSize<Type>();

			m_givenName = givenName;
			m_eReflectType = eType;
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
				Log_Error("[ReflectTypeCPP::Copy_s] src: '%p' or dst: '%p' are not valid.", src, dst);
			}
			else if (sizeof(value_type) != dst_size)
			{
				Log_Error("[ReflectTypeCPP::Copy_s] dst_size: '%i' does not match value_type size: '%i'.", dst_size, sizeof(value_type));
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

		ReflectTypeCPP(EReflectType eType, std::string givenName = "")
		{
			m_typeName = "void";
			m_typeSize = 0;

			m_valueTypeName = "void";
			m_valueTypeSize = 0;

			m_givenName = givenName;
			m_eReflectType = eType;
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

	struct ReflectTypeNameData
	{
		std::string Type;
		std::string RawType;
		std::string Name;
		EReflectValueType ReflectValueType;
		EReflectValueModifier ReflectModifier;
		std::vector<std::string> ContainerProps;

		int TypeSize;
		bool IsConst;

		ReflectTypeNameData()
			: Type("")
			, Name("")

			, TypeSize(0)
			, IsConst(false)
		{ }

		template<typename T>
		ReflectTypeNameData(const std::string& type, const std::string& rawType, const std::string& name, const int& typeSize, const Reflect::EReflectValueType& memberType, const bool& isConst)
			: Type(type)
			, Name(name)
			, RawType(rawType)
			, TypeSize(typeSize)
			, ReflectValueType(memberType)
			, IsConst(isConst)
		{ }

		bool operator!=(const ReflectTypeNameData& other) const
		{
			bool propsEqual = ContainerProps.size() != other.ContainerProps.size();
			if (propsEqual)
			{
				for (uint32_t i = 0; i < ContainerProps.size(); ++i)
				{
					if (ContainerProps[i] == other.ContainerProps[i])
					{
						propsEqual = false;
						break;
					}
				}
			}
			return Type != other.Type ||
				Name != other.Name ||
				TypeSize != other.TypeSize ||
				propsEqual;
		}
	};

	struct ReflectMemberData : public ReflectTypeNameData
	{
		EReflectType ReflectType = EReflectType::Member;
		int Offset = 0;
	};

	struct ReflectFunctionData : public ReflectTypeNameData
	{
		EReflectType ReflectType = EReflectType::Function;
		std::vector<ReflectTypeNameData> Parameters;
	};

	struct ReflectContainerData : public ReflectTypeNameData
	{
		std::string Name;
		std::string PrettyName;
		std::vector<std::string> Inheritance;
		EReflectType ReflectType;
		int ReflectGenerateBodyLine;

		std::vector<ReflectMemberData> Members;
		std::vector<ReflectFunctionData> Functions;
	};

	struct FileParsedData
	{
		std::string Data;
		int Cursor;
		std::string FilePath;
		std::string FileName;
		int GeneratedBodyLineOffset;
		std::vector<std::string> CPPIncludes;

		std::vector<ReflectContainerData> ReflectData;
	};

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

		Arg GetArg(int index) const
		{
			return m_args.at(index);
		}

		template<typename T>
		void AddArg(T* obj)
		{
			m_args.push_back(Arg(Reflect::Util::GetTypeName(*obj), obj));
		}

		void Clear() { m_args.clear(); }

		int GetSize() const { return static_cast<int>(m_args.size()); }

	private:
		std::vector<Arg> m_args;
	};

	using FunctionPtr = Reflect::EReflectReturnCode(*)(void* objectPtr, void* returnValue, FunctionPtrArgs& args);

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

	struct REFLECT_API IReflect
	{
		virtual ReflectFunction GetFunction(const char* functionName) { (void)functionName; return ReflectFunction(nullptr, nullptr);};
		virtual ReflectMember GetMember(const char* memberName) { (void)memberName; return ReflectMember("", nullptr, nullptr); };
		virtual std::vector<ReflectMember> GetMembers(std::vector<std::string> const& flags) { (void)flags; return {}; };
		virtual std::vector<ReflectMember> GetAllMembers() { return {}; };
	};

#define REFLET_TYPE_INFO
#ifdef REFLET_TYPE_INFO
	template<typename T>
	class GenerateTypeInfoForType;

	class REFLECT_API ReflectTypeInfo;

	class REFLECT_API ReflectTypeMember
	{
	public:
		ReflectTypeMember(void* ownerClass, void* memberPtr, std::unique_ptr<ReflectType> type,
			std::vector<std::string> flags);

		ReflectType* GetType() const;
		bool IsValid() const;
		bool HasFlag(const char* flag) const;

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
		std::unique_ptr<ReflectType> m_type;
		std::vector<std::string> m_flags;

		friend class ReflectTypeInfo;
	};

	class REFLECT_API ReflectTypeFunction
	{
	public:
		ReflectTypeFunction(void* ownerClass, FunctionPtr funcPtr
			, std::unique_ptr<ReflectType> info, std::vector<std::unique_ptr<ReflectType>> args);

		Reflect::EReflectReturnCode Invoke(FunctionPtrArgs functionArgs = FunctionPtrArgs());
		//TODO: FunctionPtr returnValuePointer needs to be a pointer to a pointer. This will allow pointers to be returned from functions.
		template<typename T>
		Reflect::EReflectReturnCode Invoke(T* returnValue, FunctionPtrArgs functionArgs = FunctionPtrArgs())
		{
			return CallInternal((void*)returnValue, std::make_unique<ReflectTypeCPP<T>>(EReflectType::Unknown), std::move(functionArgs));
		}
		
		bool IsValid() const;

		ReflectType* GetInfo() const;
		const ReflectType* GetArgInfo(int index) const;
		std::vector<ReflectType*> GetArgsInfo() const;

	protected:
		EReflectReturnCode CallInternal(void* returnValue, std::unique_ptr<ReflectType> returnType,FunctionPtrArgs functionArgs);

	private:
		bool VerifyArgs(const FunctionPtrArgs& functionArgs) const;
		bool CheckReturnType(ReflectType* returnType) const;

	protected:
		void* m_ownerClass = nullptr;
		FunctionPtr m_func = nullptr;

		std::unique_ptr<ReflectType> m_info;
		int m_numOfArgs;
		std::vector<std::unique_ptr<ReflectType>> m_argsInfo;

		friend class ReflectTypeInfo;
	};

	/// @brief Describe from a high level the class/struct. This should contain basic info such as object name, size, 
	class REFLECT_API ReflectTypeInfo
	{
		using ConstructFunc = void*(*)();

	public:
		ReflectTypeInfo(void* owner_class, std::unique_ptr<ReflectType> info
			, std::vector<std::unique_ptr<ReflectTypeMember>> members
			, std::vector<std::unique_ptr<ReflectTypeFunction>> functions);

		/// @brief Return type info.
		/// @return ReflectType*
		ReflectType* GetInfo() const;
		
		/// @brief Construct a new object. Creates a heap allocated object with 'new'. Reflect does not manage the lifetime of this object.
		/// @return void*.
		void* ConstructNew() { return m_construct_func(); }

		/// @brief Return a ReflectTypeMember* if found.
		/// @return ReflectTypeMember*
		ReflectTypeMember* GetMember(const char* memberName) const;

		/// @brief Return all members.
		/// @return std::vector<ReflectTypeMember*>.
		std::vector<ReflectTypeMember*> GetAllMembers() const;

		/// @brief Return all members with flags given.
		/// @return std::vector<ReflectTypeMember*>.
		std::vector<ReflectTypeMember*> GetAllMembersWithFlags(std::vector<const char*> flags) const;

		/// @brief Return a ReflectTypeFunction* if found.
		/// @return ReflectTypeFunction*.
		ReflectTypeFunction* GetFunction(const char* functionName) const;

	private:
		/// @breif Pointer to an instance of which this TypeInfo is made from. This will be nullptr is no instance pointer is given.
		void* m_owner_class = nullptr; 

		/// @brief Construct function.
		ConstructFunc m_construct_func;
		
		/// @brief 
		std::unique_ptr<ReflectType> m_info;
		/// @brief Store all the memberse for this type.
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
#endif
}

#define REFLECT_BASE() public Reflect::IReflect
#define REFLECT_STRUCT(...)
#define REFLECT_CLASS(...)
