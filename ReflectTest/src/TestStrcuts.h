#pragma once

#include "Reflect.h"

REFLECT_CLASS(Person, Public, Private)
{
	REFLECT_PROPERTY(Friend, private,		)
	int x;
};

REFLECT_CLASS(House, Public, Private)
{
	REFLECT_PROPERTY(Friend, )
		int width;
};