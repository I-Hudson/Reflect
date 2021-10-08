#include "TestStrcuts.h"

int Player::GetOnlineFriendsCount(int const& maxPlayerCount) const
{
	return maxPlayerCount;
}

std::string Player::GetId() const
{
	return Id;
}

void Player::PrintHelloWorld()
{
	printf("Hello World!");
}