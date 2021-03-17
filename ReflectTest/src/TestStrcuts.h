#pragma once

#include "Reflect.h"

REFLECT_CLASS(Person, Public, Private)
{
	int x;
	int y;
	const char* TestString;
};