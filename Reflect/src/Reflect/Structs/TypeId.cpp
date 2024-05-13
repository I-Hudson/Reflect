#include "Reflect/Structs/TypeId.h"

#ifdef REFLECT_TYPE_INFO_ENABLED
namespace Reflect
{
	//-------------------------------------
	// ReflectTypeId
	//-------------------------------------
	ReflectTypeId::ReflectTypeId()
	{ }

	ReflectTypeId::ReflectTypeId(std::string typeName)
		: m_typeName(std::move(typeName))
		, m_hash(std::hash<std::string>()(m_typeName))
	{ }

	ReflectTypeId::~ReflectTypeId()
	{ }

	bool ReflectTypeId::operator==(const ReflectTypeId & other) const
	{
		return m_typeName == other.m_typeName && m_hash == other.m_hash;
	}

	bool ReflectTypeId::operator!=(const ReflectTypeId& other) const
	{
		return !(*this == other);
	}

	ReflectTypeId::operator bool() const
	{
		return IsValid();
	}

	bool ReflectTypeId::IsValid() const
	{
		return !m_typeName.empty() && m_hash != 0;
	}

	std::string_view ReflectTypeId::GetTypeName() const
	{
		return m_typeName;
	}

	uint64_t ReflectTypeId::GetTypeHash() const
	{
		return m_hash;
	}

	//-------------------------
	// TypeId
	//-------------------------
	TypeId::TypeId()
	{
	}

	TypeId::TypeId(std::string_view typeName)
		: m_typeName(typeName)
		, m_hash(std::hash<std::string>()(m_typeName))
	{
	}

	TypeId::~TypeId()
	{
	}

	TypeId::operator bool() const
	{
		return IsValid();
	}

	bool TypeId::IsValid() const
	{
		return !m_typeName.empty() && m_hash != 0;
	}

	bool TypeId::operator==(const TypeId& other) const
	{
		return m_hash == other.m_hash;
	}

	bool TypeId::operator!=(const TypeId& other) const
	{
		return !(*this == other);
	}

	std::string_view TypeId::GetTypeName() const
	{
		return m_typeName;
	}

	uint64_t TypeId::GetHash() const
	{
		return m_hash;
	}
}
#endif