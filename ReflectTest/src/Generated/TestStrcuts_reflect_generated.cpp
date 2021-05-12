#include "../TestStrcuts.h"

Reflect::ReflectMemberProp S::__REFLECT_MEMBER_PROPS__[2] = {
	Reflect::ReflectMemberProp("Friends", Reflect::Util::GetTypeName<int>(), __REFLECT__Friends(), {"EditorOnly", "Public"}),
	Reflect::ReflectMemberProp("TimeOnline", Reflect::Util::GetTypeName<int>(), __REFLECT__TimeOnline(), {"Public"}),
};

Reflect::ReflectFunction S::GetFunction(const char* functionName)
{
	return Reflect::ReflectFunction(nullptr, nullptr);
}

Reflect::ReflectMember S::GetMember(const char* memberName)
{
	for(const auto& member : __REFLECT_MEMBER_PROPS__)
	{
		if(memberName == member.Name)
		{
			//CheckFlags
			return Reflect::ReflectMember(member.Name, member.Type, ((char*)this) + member.Offset);
		}
	}
	return Reflect::ReflectMember("null", Reflect::Util::GetTypeName<void>(), nullptr);
}

std::vector<Reflect::ReflectMember> S::GetMembers(std::vector<std::string> const& flags)
{
	std::vector<Reflect::ReflectMember> members;
	for(auto& member : __REFLECT_MEMBER_PROPS__)
	{
		if(member.ContainsProperty(flags))
		{
			members.push_back(Reflect::ReflectMember(member.Name, member.Type, ((char*)this) + member.Offset));
		}
	}
	return members;
}

Reflect::ReflectMemberProp Player::__REFLECT_MEMBER_PROPS__[2] = {
	Reflect::ReflectMemberProp("Friends", Reflect::Util::GetTypeName<int>(), __REFLECT__Friends(), {"EditorOnly", "Public"}),
	Reflect::ReflectMemberProp("TimeOnline", Reflect::Util::GetTypeName<int>(), __REFLECT__TimeOnline(), {"Public"}),
};

Reflect::ReflectFunction Player::GetFunction(const char* functionName)
{
	if(functionName == "GetOnlineFriendsCount")
	{
		return Reflect::ReflectFunction(this, Player::__REFLECT_FUNC__GetOnlineFriendsCount);
	}
	if(functionName == "PrintHelloWorld")
	{
		return Reflect::ReflectFunction(this, Player::__REFLECT_FUNC__PrintHelloWorld);
	}
	if(functionName == "GetId")
	{
		return Reflect::ReflectFunction(this, Player::__REFLECT_FUNC__GetId);
	}
	return Reflect::ReflectFunction(nullptr, nullptr);
}

Reflect::ReflectMember Player::GetMember(const char* memberName)
{
	for(const auto& member : __REFLECT_MEMBER_PROPS__)
	{
		if(memberName == member.Name)
		{
			//CheckFlags
			return Reflect::ReflectMember(member.Name, member.Type, ((char*)this) + member.Offset);
		}
	}
	return Reflect::ReflectMember("null", Reflect::Util::GetTypeName<void>(), nullptr);
}

std::vector<Reflect::ReflectMember> Player::GetMembers(std::vector<std::string> const& flags)
{
	std::vector<Reflect::ReflectMember> members;
	for(auto& member : __REFLECT_MEMBER_PROPS__)
	{
		if(member.ContainsProperty(flags))
		{
			members.push_back(Reflect::ReflectMember(member.Name, member.Type, ((char*)this) + member.Offset));
		}
	}
	return members;
}

