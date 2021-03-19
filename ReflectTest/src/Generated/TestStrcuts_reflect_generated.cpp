#include "../TestStrcuts.h"
PersonReflectObject::PersonReflectObject()
{}

PersonReflectObject::~PersonReflectObject()
{}

void PersonReflectObject::ReflectInit()
{}

void PersonReflectObject::SetupReflectBindings()
{
	if (m_reflectInit)
	{
		return;
	}
	m_reflectInit = true;
}

ReflectFunction PersonReflectObject::GetFunction(const char* name)
{
	return __super::GetFunctionBase(name);
}

ReflectMember PersonReflectObject::GetMember(const char* name)
{
	return __super::GetMemberBase(name);
}

HouseReflectObject::HouseReflectObject()
{}

HouseReflectObject::~HouseReflectObject()
{}

void HouseReflectObject::ReflectInit()
{}

void HouseReflectObject::SetupReflectBindings()
{
	if (m_reflectInit)
	{
		return;
	}
	m_reflectInit = true;
}

ReflectFunction HouseReflectObject::GetFunction(const char* name)
{
	return __super::GetFunctionBase(name);
}

ReflectMember HouseReflectObject::GetMember(const char* name)
{
	return __super::GetMemberBase(name);
}

