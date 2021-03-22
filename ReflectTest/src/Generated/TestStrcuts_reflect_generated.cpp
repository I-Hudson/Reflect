#include "../TestStrcuts.h"

ReflectMember Person::GetMember(const char* memberName)
{
	if(memberName == "y")
	{
		return ReflectMember("y", Reflect::Util::GetTypeName(y), &y);
	}
	if(memberName == "x")
	{
		return ReflectMember("x", Reflect::Util::GetTypeName(x), &x);
	}
	if(memberName == "a")
	{
		return ReflectMember("a", Reflect::Util::GetTypeName(a), &a);
	}
	if(memberName == "w")
	{
		return ReflectMember("w", Reflect::Util::GetTypeName(w), &w);
	}
	if(memberName == "h")
	{
		return ReflectMember("h", Reflect::Util::GetTypeName(h), &h);
	}
	if(memberName == "u")
	{
		return ReflectMember("u", Reflect::Util::GetTypeName(u), &u);
	}
	if(memberName == "c")
	{
		return ReflectMember("c", Reflect::Util::GetTypeName(c), &c);
	}
	if(memberName == "TestString")
	{
		return ReflectMember("TestString", Reflect::Util::GetTypeName(TestString), &TestString);
	}
	return ReflectMember("null", Reflect::Util::GetTypeName<void>(), nullptr);
}

ReflectMember House::GetMember(const char* memberName)
{
	if(memberName == "width")
	{
		return ReflectMember("width", Reflect::Util::GetTypeName(width), &width);
	}
	if(memberName == "m_person")
	{
		return ReflectMember("m_person", Reflect::Util::GetTypeName(m_person), &m_person);
	}
	return ReflectMember("null", Reflect::Util::GetTypeName<void>(), nullptr);
}

