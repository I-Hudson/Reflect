#pragma once

#include "Reflect.h"
#include "Generated/TestStrcuts_reflect_generated.h"


REFLECT_CLASS(Person, Public, Private)
{
public:
	Person() { ReflectInit(); }

	REFLECT_PROPERTY(Friend, private)
	int x;
};

REFLECT_CLASS(House, Public, Private)
{
	REFLECT_PROPERTY(Friend, )
		int width;
};