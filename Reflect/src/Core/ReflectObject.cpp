#include "ReflectObject.h"

ReflectObject::ReflectObject()
	: m_reflectInit(false)
{ }

ReflectObject::~ReflectObject()
{ }

ReflectFunction ReflectObject::GetFunctionBase(const char* name)
{
	SetupReflectBindings();
	return ReflectFunction();
}

ReflectMember ReflectObject::GetMemberBase(const char* name)
{
	SetupReflectBindings();
	return ReflectMember();
}

void ReflectObject::AddFunction(const char* functionName, void* functionPtr)
{
}

void ReflectObject::AddMember(const char* memberName, void* memberPtr)
{
}