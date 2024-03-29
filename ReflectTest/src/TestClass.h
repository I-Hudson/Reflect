#pragma once

#include "Reflect.h"
#include "Generated/TestClass_reflect_generated.h"

#define ENABLED
#ifdef ENABLED

REFLECT_CLASS()
class BaseClass : REFLECT_BASE
{
	REFLECT_GENERATED_BODY();

	std::string GetName() const { return "BaseClass"; }
};

REFLECT_CLASS()
class ClassToRefectA : REFLECT_BASE
{
	REFLECT_GENERATED_BODY();

	std::string GetName() const { return "ClassToRefectA"; }
};

REFLECT_CLASS()
class ClassToRefectB : BaseClass
{
	REFLECT_GENERATED_BODY();

	std::string GetName() const { return "ClassToRefectB"; }
};

REFLECT_CLASS()
class ClassToRefect : REFLECT_BASE
{
	REFLECT_GENERATED_BODY();

	std::string GetName() const { return "ClassToRefect"; }
};

#endif

#ifdef ENABLED
REFLECT_CLASS()
class ClassA : public ClassToRefect
{
	REFLECT_GENERATED_BODY();

	std::string GetName() const { return "ClassA"; }
};

REFLECT_CLASS()
class ClassB : public ClassA
{
	REFLECT_GENERATED_BODY();

	std::string GetName() const { return "ClassB"; }
};

REFLECT_CLASS()
class ClassC : public ClassB
{

	REFLECT_GENERATED_BODY();

	std::string GetName() const { return "ClassC"; }
};

REFLECT_CLASS()
class ClassD : public ClassC, public ClassToRefectA, public ClassToRefectB
{
	REFLECT_GENERATED_BODY();

	std::string GetName() const { return "ClassD"; }
};


REFLECT_CLASS()
class ClassE : public ClassD
{
	REFLECT_GENERATED_BODY();

	std::string GetName() const { return "ClassE"; }
};

REFLECT_CLASS()
class ClassHolder : REFLECT_BASE
{
	REFLECT_GENERATED_BODY();

	REFLECT_PROPERTY()
	ClassE ClassEObject;
	REFLECT_PROPERTY()
	ClassB ClassBObject;
};
#endif