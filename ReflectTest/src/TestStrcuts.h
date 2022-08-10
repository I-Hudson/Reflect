#pragma once

#include "Reflect.h"
#include "Generated/TestStrcuts_reflect_generated.h"

struct vec3
{
	vec3(float f)
	{ }

	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		struct
		{
			float v[3];
		};
	};
};

class Interface
{

};

/// <summary>
/// Example class.
/// </summary>

REFLECT_STRUCT()
struct S : REFLECT_BASE(), public Interface
{
	REFLECT_GENERATED_BODY()

	int& GetFriends() { return Friends; }

	int Friends = int(1);
	int* TimeOnline = nullptr;
	vec3 vec = vec3(1);
};

class C
{

};

REFLECT_CLASS(AllPrivate, ShowInEditorOnly, EditorOnly)
class Player : REFLECT_BASE()
{
	REFLECT_GENERATED_BODY();

public:
	Player()
		: Id("PlayerExampleId")
	{ }

	~Player(){}

	REFLECT_PROPERTY(Public)
	int GetOnlineFriendsCount(int const& maxPlayerCount)const;

	REFLECT_PROPERTY()
	void PrintHelloWorld();

	std::string GetName() { return "Test name"; }

private:
	REFLECT_PROPERTY()
	std::string GetId();

	REFLECT_PROPERTY()
	std::string* GetIdPtr();

private:
	std::string Id = { };

	REFLECT_PROPERTY(EditorOnly, Public)
		int Friends;
	REFLECT_PROPERTY(Public)
		int TimeOnline = 78;
};