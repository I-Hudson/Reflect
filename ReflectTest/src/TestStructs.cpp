#include "TestStrcuts.h"


	void Player::SetAgeAndHeight(int newAge, float newHeight)
	{
		Age = newAge;
		Height = newHeight;
	}

	int Player::GetOnlineFriendsCount(int const& maxPlayerCount) const
	{
		return maxPlayerCount;
	}

	std::string Player::GetId()
	{
		return Id;
	}

	std::string* Player::GetIdPtr()
	{
		return &Id;
	}

	void Player::PrintHelloWorld()
	{
		printf("Hello World!");
	}