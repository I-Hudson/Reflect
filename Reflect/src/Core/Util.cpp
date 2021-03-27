#include "Util.h"

NAMESPACE_START

std::string Util::ValidateTypeName(const std::string& str)
{
	int len = (int)str.length();
	int index = len - 1;
	while (str[index] != ' ' && str[index] != str[0])
	{
		--index;
	}

	if (index == 0)
	{
		return str;
	}
	return str.substr(0, index);
}

NAMESPACE_END