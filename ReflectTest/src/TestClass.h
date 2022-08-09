#pragma once

#include "Reflect.h"
#include "Generated/TestClass_reflect_generated.h"

REFLECT_CLASS()
class ClassToRefect : REFLECT_BASE()
{
	REFLECT_GENERATED_BODY();

	std::string GetName() const { return "ClassToReflect"; }
};