 // This file is auto generated please don't modify.
#include "Core/ReflectObject.h"

#ifdef TestStrcuts_reflect_generated_h
#error "TestStrcuts_reflect_generated.h already included, missing 'pragma once' in TestStrcuts.h"
#endif TestStrcuts_reflect_generated_h
#define TestStrcuts_reflect_generated_h

#define TestStrcuts_Source_h_9_PROPERTIES_OFFSET \
	static int __REFLECT__y() { return offsetof(Person, y); }; \
	static int __REFLECT__x() { return offsetof(Person, x); }; \
	static int __REFLECT__TestString() { return offsetof(Person, TestString); }; \

#define TestStrcuts_Source_h_9_PROPERTIES_GET \
public:\
ReflectMember GetMember(const char* memberName)\
{\
	if(memberName == "y")\
	{\
		return ReflectMember(&y);\
	}\
	if(memberName == "x")\
	{\
		return ReflectMember(&x);\
	}\
	if(memberName == "TestString")\
	{\
		return ReflectMember(&TestString);\
	}\
}\
private:

#define TestStrcuts_Source_h_9_GENERATED_BODY \
TestStrcuts_Source_h_9_PROPERTIES_OFFSET \
TestStrcuts_Source_h_9_PROPERTIES_GET

#define TestStrcuts_Source_h_31_PROPERTIES_OFFSET \
	static int __REFLECT__width() { return offsetof(House, width); }; \

#define TestStrcuts_Source_h_31_PROPERTIES_GET \
public:\
ReflectMember GetMember(const char* memberName)\
{\
	if(memberName == "width")\
	{\
		return ReflectMember(&width);\
	}\
}\
private:

#define TestStrcuts_Source_h_31_GENERATED_BODY \
TestStrcuts_Source_h_31_PROPERTIES_OFFSET \
TestStrcuts_Source_h_31_PROPERTIES_GET

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID TestStrcuts_Source_h
