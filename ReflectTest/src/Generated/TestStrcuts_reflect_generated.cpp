#include "../TestStrcuts.h"

Reflect::ReflectMemberProp Person::__REFLECT_MEMBER_PROPS__[3] = {
	Reflect::ReflectMemberProp("y", Reflect::Util::GetTypeName<int>(), __REFLECT__y(), 2),
	Reflect::ReflectMemberProp("x", Reflect::Util::GetTypeName<int>(), __REFLECT__x(), 2),
	Reflect::ReflectMemberProp("TestString", Reflect::Util::GetTypeName<std::string>(), __REFLECT__TestString(), 0),
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
	for(const auto& member : __REFLECT_MEMBER_PROPS__)
	{
		if(memberName == member.Name)
		{
			//CheckFlags
			return ReflectMember(member.Name, member.Type, ((char*)this) + member.Offset);
		}
	}
	return ReflectMember("null", Reflect::Util::GetTypeName<void>(), nullptr);
}

Reflect::ReflectMemberProp House::__REFLECT_MEMBER_PROPS__[2] = {
	Reflect::ReflectMemberProp("width", Reflect::Util::GetTypeName<int>(), __REFLECT__width(), 8),
	Reflect::ReflectMemberProp("m_person", Reflect::Util::GetTypeName<Person>(), __REFLECT__m_person(), 0),
};

ReflectFunction House::GetFunction(const char* functionName)
{
	return ReflectFunction(nullptr, nullptr);
}

ReflectMember House::GetMember(const char* memberName)
{
	for(const auto& member : __REFLECT_MEMBER_PROPS__)
	{
		if(memberName == member.Name)
		{
			//CheckFlags
			return ReflectMember(member.Name, member.Type, ((char*)this) + member.Offset);
		}
	}
	return ReflectMember("null", Reflect::Util::GetTypeName<void>(), nullptr);
}

