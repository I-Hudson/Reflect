#pragma once

#include "Reflect/Core/Defines.h"

#include <string>
#include <xhash>

#ifdef REFLECT_TYPE_INFO_ENABLED
namespace Reflect
{
	/// @brief Unique Id for a given type.
	class REFLECT_API ReflectTypeId
	{
	public:
		ReflectTypeId();
		ReflectTypeId(std::string typeName);
		~ReflectTypeId();

		operator bool() const;
		bool operator==(const ReflectTypeId& other) const;
		bool operator!=(const ReflectTypeId& other) const;

		bool IsValid() const;
		std::string_view GetTypeName() const;
		uint64_t GetTypeHash() const;

	private:
		std::string m_typeName;
		uint64_t m_hash = 0;
	};

	/// @brief A TypeId is a struct used as a unique identifier of a type.
	struct REFLECT_API TypeId
	{
	public:
		TypeId();
		TypeId(std::string_view typeName);
		~TypeId();

		template<typename T>
		static TypeId MakeTypeId()
		{
			TypeId typeId(Util::GetValueTypeName<T>());
			return typeId;
		}

		operator bool() const;
		bool IsValid() const;
		
		bool operator==(const TypeId& other) const;
		bool operator!=(const TypeId& other) const;

		std::string_view GetTypeName() const;
		uint64_t GetHash() const;

	private:
		std::string m_typeName;
		uint64_t m_hash = 0;
	};
}

namespace std
{
	template<>
	struct hash<Reflect::ReflectTypeId>
	{
		std::size_t operator()(const Reflect::ReflectTypeId& typeId) const noexcept
		{
			return typeId.GetTypeHash();
		}
	};

	template<>
	struct hash<Reflect::TypeId>
	{
		std::size_t operator()(const Reflect::TypeId& typeId) const noexcept
		{
			return typeId.GetHash();
		}
	};
}
#endif