#pragma once

#include "Reflect/Core/Core.h"

#include <string>
#include <unordered_map>

namespace Reflect
{
	struct ReflectAddtionalOptions
	{
		std::string_view GetOption(std::string_view optionKey) const
		{
			if (auto itr = options.find(optionKey.data()); itr != options.end())
			{
				return itr->second;
			}
			return "";
		}

		std::unordered_map<std::string, std::string> options =
		{
			{ Reflect::Keys::Reflect_CMD_Option_PCH_Include, "" },
			{ Reflect::Keys::Reflect_CMD_Option_Reflect_Full_EXT, "false" },
			//{ Reflect::Reflect_CMD_Option_Single_File_EXT, "false" }

		};
	};
}