#pragma once

#include "Reflect.h"
#include "Generated/TestStrcuts_reflect_generated.h"

/// <summary>
/// Example class.
/// </summary>
REFLECT_CLASS(Player)
{
	REFLECT_GENERATED_BODY()

public:
	Player()
		: Id("PlayerExampleId")
	{ }

	~Player()
	{ }

	REFLECT_PROPERTY()
		int GetOnlineFriendsCount(int const& maxPlayerCount);

	REFLECT_PROPERTY()
		void PrintHelloWorld();

private:
	REFLECT_PROPERTY()
		std::string GetId() const;

private:
	std::string Id;

	REFLECT_PROPERTY()
		int Friends;
	REFLECT_PROPERTY()
		int TimeOnline;
};