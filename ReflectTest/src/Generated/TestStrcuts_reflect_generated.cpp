#include "../TestStrcuts.h"

Reflect::ReflectMemberProp Player::__REFLECT_MEMBER_PROPS__[2] = {
	Reflect::ReflectMemberProp("Friends", Reflect::Util::GetTypeName<int>(), __REFLECT__Friends(), 0),
	Reflect::ReflectMemberProp("TimeOnline", Reflect::Util::GetTypeName<int>(), __REFLECT__TimeOnline(), 0),
};

ReflectFunction Player::GetFunction(const char* functionName)
{
	if(functionName == "GetOnlineFriendsCount")
	{
		return ReflectFunction(this, Player::__REFLECT_FUNC__GetOnlineFriendsCount);
	}
	if(functionName == "PrintHelloWorld")
	{
		return ReflectFunction(this, Player::__REFLECT_FUNC__PrintHelloWorld);
	}
	if(functionName == "GetId")
	{
		return ReflectFunction(this, Player::__REFLECT_FUNC__GetId);
	}
	return ReflectFunction(nullptr, nullptr);
}

ReflectMember Player::GetMember(const char* memberName)
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

std::vector<ReflectMember> Player::GetMembers(unsigned int flags)
{
	std::vector<ReflectMember> members;
	for(const auto& member : __REFLECT_MEMBER_PROPS__)
	{
		if(flags & member.Properties)
		{
			members.push_back(ReflectMember(member.Name, member.Type, ((char*)this) + member.Offset));
		}
	}
	return members;
}

