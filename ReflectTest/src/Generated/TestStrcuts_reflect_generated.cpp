#include "../TestStrcuts.h"

Reflect::ReflectMemberProp Person::__REFLECT_MEMBER_PROPS__[8] = {
	Reflect::ReflectMemberProp("y", __REFLECT__y(), 2),
	Reflect::ReflectMemberProp("x", __REFLECT__x(), 8),
	Reflect::ReflectMemberProp("a", __REFLECT__a(), 4),
	Reflect::ReflectMemberProp("w", __REFLECT__w(), 10),
	Reflect::ReflectMemberProp("h", __REFLECT__h(), 8),
	Reflect::ReflectMemberProp("u", __REFLECT__u(), 6),
	Reflect::ReflectMemberProp("c", __REFLECT__c(), 12),
	Reflect::ReflectMemberProp("TestString", __REFLECT__TestString(), 0),
};

ReflectFunction Person::GetFunction(const char* functionName)
{
	if(functionName == "TestFuncInt")
	{
		return ReflectFunction(this, Person::__REFLECT_FUNC__TestFuncInt);
	}
	if(functionName == "TestFuncChar")
	{
		return ReflectFunction(this, Person::__REFLECT_FUNC__TestFuncChar);
	}
	if(functionName == "TestFuncVoid")
	{
		return ReflectFunction(this, Person::__REFLECT_FUNC__TestFuncVoid);
	}
	return ReflectFunction(nullptr, nullptr);
}

ReflectMember Person::GetMember(const char* memberName)
{
	int propsIndex = 0;
	if(memberName == "y")
	{
		//if()
		{
			return ReflectMember("y", Reflect::Util::GetTypeName(y), &y);
		}
	}
	++propsIndex;
	if(memberName == "x")
	{
		//if()
		{
			return ReflectMember("x", Reflect::Util::GetTypeName(x), &x);
		}
	}
	++propsIndex;
	if(memberName == "a")
	{
		//if()
		{
			return ReflectMember("a", Reflect::Util::GetTypeName(a), &a);
		}
	}
	++propsIndex;
	if(memberName == "w")
	{
		//if()
		{
			return ReflectMember("w", Reflect::Util::GetTypeName(w), &w);
		}
	}
	++propsIndex;
	if(memberName == "h")
	{
		//if()
		{
			return ReflectMember("h", Reflect::Util::GetTypeName(h), &h);
		}
	}
	++propsIndex;
	if(memberName == "u")
	{
		//if()
		{
			return ReflectMember("u", Reflect::Util::GetTypeName(u), &u);
		}
	}
	++propsIndex;
	if(memberName == "c")
	{
		//if()
		{
			return ReflectMember("c", Reflect::Util::GetTypeName(c), &c);
		}
	}
	++propsIndex;
	if(memberName == "TestString")
	{
		//if()
		{
			return ReflectMember("TestString", Reflect::Util::GetTypeName(TestString), &TestString);
		}
	}
	++propsIndex;
	return ReflectMember("null", Reflect::Util::GetTypeName<void>(), nullptr);
}

Reflect::ReflectMemberProp House::__REFLECT_MEMBER_PROPS__[2] = {
	Reflect::ReflectMemberProp("width", __REFLECT__width(), 8),
	Reflect::ReflectMemberProp("m_person", __REFLECT__m_person(), 0),
};

ReflectFunction House::GetFunction(const char* functionName)
{
	return ReflectFunction(nullptr, nullptr);
}

ReflectMember House::GetMember(const char* memberName)
{
	int propsIndex = 0;
	if(memberName == "width")
	{
		//if()
		{
			return ReflectMember("width", Reflect::Util::GetTypeName(width), &width);
		}
	}
	++propsIndex;
	if(memberName == "m_person")
	{
		//if()
		{
			return ReflectMember("m_person", Reflect::Util::GetTypeName(m_person), &m_person);
		}
	}
	++propsIndex;
	return ReflectMember("null", Reflect::Util::GetTypeName<void>(), nullptr);
}

