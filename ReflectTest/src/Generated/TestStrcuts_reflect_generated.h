 // This file is auto generated please don't modify.
#include "Core/ReflectObject.h"
#include <assert.h>
class PersonReflectObject : ReflectObject
{
public:
	PersonReflectObject();
	~PersonReflectObject();
	void ReflectInit();
	virtual const char* Verify() override { return "Person"; } 
	virtual ReflectFunction GetFunction(const char* name) override;
	virtual ReflectMember GetMember(const char* name) override;
protected:
	virtual void SetupReflectBindings() override;
};

class HouseReflectObject : ReflectObject
{
public:
	HouseReflectObject();
	~HouseReflectObject();
	void ReflectInit();
	virtual const char* Verify() override { return "House"; } 
	virtual ReflectFunction GetFunction(const char* name) override;
	virtual ReflectMember GetMember(const char* name) override;
protected:
	virtual void SetupReflectBindings() override;
};

