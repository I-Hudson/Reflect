#include "TestStrcuts.h"

int Player::GetOnlineFriendsCount(int const& maxPlayerCount) const
{
	return maxPlayerCount;
}

std::string* Player::GetId()
{
	return &Id;
}

void Player::PrintHelloWorld()
{
	printf("Hello World!");
}