#pragma once

#include "Core/Defines.h"

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
	using TypeId = ReflectTypeId;
}

namespace std
{
	template<>
	struct hash<Reflect::TypeId>
	{
		std::size_t operator()(const Reflect::ReflectTypeId& typeId) const noexcept
		{
			return typeId.GetTypeHash();
		}
	};
}
#endif