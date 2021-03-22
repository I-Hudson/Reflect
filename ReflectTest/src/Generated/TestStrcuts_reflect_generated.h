 // This file is auto generated please don't modify.
#include "Core/ReflectObject.h"
#include "Core/Util.h"

#ifdef TestStrcuts_reflect_generated_h
#error "TestStrcuts_reflect_generated.h already included, missing 'pragma once' in TestStrcuts.h"
#endif TestStrcuts_reflect_generated_h
#define TestStrcuts_reflect_generated_h

#define TestStrcuts_Source_h_9_FUNCTION_MEMBERS \
private:\
	int __REFLECT_FUNC_MEMBER__TestFuncInt;\
	const char* __REFLECT_FUNC_MEMBER__TestFuncChar;\


#define TestStrcuts_Source_h_9_PROPERTIES_OFFSET \
private:\
	static int __REFLECT__y() { return offsetof(Person, y); }; \
	static int __REFLECT__x() { return offsetof(Person, x); }; \
	static int __REFLECT__a() { return offsetof(Person, a); }; \
	static int __REFLECT__w() { return offsetof(Person, w); }; \
	static int __REFLECT__h() { return offsetof(Person, h); }; \
	static int __REFLECT__u() { return offsetof(Person, u); }; \
	static int __REFLECT__c() { return offsetof(Person, c); }; \
	static int __REFLECT__TestString() { return offsetof(Person, TestString); }; \


#define TestStrcuts_Source_h_9_PROPERTIES_GET \
public:\
ReflectMember GetMember(const char* memberName);\
private:

#define TestStrcuts_Source_h_9_GENERATED_BODY \
TestStrcuts_Source_h_9_FUNCTION_MEMBERS \
TestStrcuts_Source_h_9_PROPERTIES_OFFSET \
TestStrcuts_Source_h_9_PROPERTIES_GET \


#define TestStrcuts_Source_h_40_FUNCTION_MEMBERS \
private:\


#define TestStrcuts_Source_h_40_PROPERTIES_OFFSET \
private:\
	static int __REFLECT__width() { return offsetof(House, width); }; \
	static int __REFLECT__m_person() { return offsetof(House, m_person); }; \


#define TestStrcuts_Source_h_40_PROPERTIES_GET \
public:\
ReflectMember GetMember(const char* memberName);\
private:

#define TestStrcuts_Source_h_40_GENERATED_BODY \
TestStrcuts_Source_h_40_FUNCTION_MEMBERS \
TestStrcuts_Source_h_40_PROPERTIES_OFFSET \
TestStrcuts_Source_h_40_PROPERTIES_GET \


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID TestStrcuts_Source_h
