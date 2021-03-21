#pragma once

#include "Core/Core.h"
#include <string>
#include <algorithm>

NAMESPACE_START

namespace Util
{
	std::string ToLower(std::string str)
	{
		std::transform(str.begin(), str.end(), str.begin(), [](char c)
			{
				return std::tolower(c);
			});

		return str;
	}
}

NAMESPACE_END