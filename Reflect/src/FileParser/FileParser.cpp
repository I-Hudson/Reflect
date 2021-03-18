#include "FileParser.h"
#include <sstream>
#include <vector>
#include <iostream>
#include <filesystem>
#include <stack>
#include <assert.h>

NAMESPACE_START

constexpr int DEFAULT_TYPE_SIZE = 0;

FileParser::FileParser()
{ }

FileParser::~FileParser()
{ }

void FileParser::ParseDirectory(const std::string& directory)
{
	for (const auto& f : std::filesystem::recursive_directory_iterator(directory))
	{
		std::string filePath = f.path().u8string();
		std::cout << filePath << std::endl;

		if ((f.is_regular_file() || f.is_character_file()) &&
			CheckExtension(filePath, { ".h", ".hpp" }))
		{
			// TODO thread this. We could load files on more than one thread to speed
			// this up.
			std::cout << f.file_size() << std::endl;
			std::ifstream file = OpenFile(filePath);
			FileParsedData data = LoadFile(file);
			m_filesParsed.push_back(data);
			CloseFile(file);
		}
	}


	// All files have been loaded.
	// Now we need to parse them to find all the information we want from them.
	// TODO this could also be threaded.
	for (auto& file : m_filesParsed)
	{
		ParseFile(file);
	}
}

std::ifstream FileParser::OpenFile(const std::string& filePath)
{
	std::ifstream file = std::ifstream(filePath, std::ios::in);
	if (!file.is_open())
	{
		//todo assert.
		assert(false && "[FileParser::OpenFile] File is not open.");
	}
	return std::move(file);
}

void FileParser::CloseFile(std::ifstream& file)
{
	if (file.is_open())
	{
		file.close();
	}
}

bool FileParser::CheckExtension(const std::string& filePath, std::vector<const char*> extensions)
{
	std::string extension = filePath.substr(filePath.find_last_of('.'));
	for (auto& e : extensions)
	{
		if (e == extension)
		{
			return true;
		}
	}
	return false;
}

FileParsedData FileParser::LoadFile(std::ifstream& file)
{
	FileParsedData data = {};

	file.seekg(0, std::ios::end);
	int fileSize = static_cast<int>(file.tellg());
	file.seekg(0, std::ios::beg);
	data.Data = std::string(fileSize, '\0');
	data.Cursor = 0;
	file.read(&data.Data[0], fileSize);

	return std::move(data);
}

void FileParser::ParseFile(FileParsedData& fileData)
{
	while (ReflectContainerHeader(fileData, RefectStructKey, ReflectType::Struct) || ReflectContainerHeader(fileData, RefectClassKey, ReflectType::Class))
	{
		ReflectContainer(fileData);
	}
}

bool FileParser::ReflectContainerHeader(FileParsedData& fileData, const std::string& keyword, const ReflectType type)
{
	// Check if we can reflect this class/struct. 
	int reflectStart = static_cast<int>(fileData.Data.find(keyword, fileData.Cursor));
	if (reflectStart == std::string::npos)
	{
		// Can't reflect this class. Return.
		return false;
	}

	ReflectContainerData containerData = {};

	containerData.ReflectType = type;
	fileData.Cursor = reflectStart + static_cast<int>(keyword.length()) + 1;

	// Get the flags passed though the REFLECT macro.
	std::string containerName;
	bool foundContainerName = false;

	while (fileData.Data[fileData.Cursor] != ',')
	{
		if (fileData.Data[fileData.Cursor] != ' ')
		{
			containerName += fileData.Data[fileData.Cursor];
		}
		++fileData.Cursor;
	}
	containerData.Name = containerName;
	containerData.Type = containerName;
	containerData.TypeSize = DEFAULT_TYPE_SIZE;
	containerData.ContainerProps = ReflectFlags(fileData);
	fileData.ReflectData.push_back(containerData);

	return true;
}

void FileParser::ReflectContainer(FileParsedData& fileData)
{
	int endOfContainerCursor = FindEndOfConatiner(fileData);

	// Good, we have a reflected container class/struct.
	// First find out which it is and verify that we are inheriting from "ReflectObject".
	std::stack<char> bracketStack;
	bool findStart = true;
	ReflectContainerData& conatinerData = fileData.ReflectData.back();

	while (true)
	{
		int reflectStart = static_cast<int>(fileData.Data.find(PropertyKey, fileData.Cursor));
		if (reflectStart == std::string::npos || reflectStart > endOfContainerCursor)
		{
			// There are no more properties to reflect or we have found a new container to reflect.
			break;
		}
		fileData.Cursor = reflectStart + static_cast<int>(strlen(PropertyKey));

		// Get the reflect flags.
		auto propFlags = ReflectFlags(fileData);

		// Get the type and name of the property to reflect.
		auto [type, name] = ReflectTypeAndName(fileData, {});

		char c = FindNextChar(fileData, { ' ' });
		// Find out if the property is a function or member variable.
		if (c == ';')
		{
			// Member
			// We have found a member variable 
			ReflectMemberData memberData = {};
			memberData.Type = type;
			memberData.Name = name;
			memberData.TypeSize = DEFAULT_TYPE_SIZE;
			memberData.ContainerProps = propFlags;
			conatinerData.Members.push_back(memberData);
		}
		else if (c == '(')
		{
			ReflectFunctionData funcData = {};
			funcData.Type = type;
			funcData.Name = name;
			funcData.TypeSize = DEFAULT_TYPE_SIZE;
			funcData.ContainerProps = propFlags;
			conatinerData.Functions.push_back(funcData);

			// Function
			ReflectGetFunctionParameters(fileData);
		}
		else
		{
			assert(false && "[FileParser::ParseFile] Unknown reflect type. This must be a member variable or function.");
		}

		++fileData.Cursor;
	}
}

int FileParser::FindEndOfConatiner(const FileParsedData& fileData)
{
	int cursor = fileData.Cursor;
	char lastCharacter = '\0';
	char c = '\0';
	while (true)
	{
		if (lastCharacter == '}' && c == ';')
		{
			break;
		}

		if (c != '\t' && c != '\n')
		{
			lastCharacter = c;
		}
		++cursor;
		c = fileData.Data[cursor];
	}
	return cursor;
}

std::vector<ReflectFlags> FileParser::ReflectFlags(FileParsedData& fileData)
{
	// Get the flags passed though the REFLECT macro.
	std::string flag;
	std::vector<Reflect::ReflectFlags> flags;

	if (fileData.Data[fileData.Cursor] == '(')
	{
		++fileData.Cursor;
	}

	while (fileData.Data[fileData.Cursor] != ')')
	{
		char c = fileData.Data[fileData.Cursor];
		if (c == ',')
		{
			if (!flag.empty())
			{
				flags.push_back(StringToReflectFlags(flag));
			}
			flag = "";
		}
		else
		{
			if (c != ' ' && c != '\t')
			{
				flag += c;
			}
		}
		++fileData.Cursor;
	}
	++fileData.Cursor;
	if (!flag.empty())
	{
		flags.push_back(StringToReflectFlags(flag));
	}

	return flags;
}

char FileParser::FindNextChar(FileParsedData& fileData, const std::vector<char>& ingoreChars)
{
	while (std::find(ingoreChars.begin(), ingoreChars.end(), fileData.Data[fileData.Cursor]) != ingoreChars.end())
	{
		++fileData.Cursor;
	}
	return fileData.Data[fileData.Cursor];
}

bool FileParser::RefectCheckForEndOfLine(const FileParsedData& fileData)
{
	char c = fileData.Data[fileData.Cursor];
	if (c == ' ' || c == '(' || c == ';')
	{
		return true;
	}

	return false;
}

bool FileParser::ReflectTypeCheck(const std::string& type)
{
	if (type == "const")
	{
		return true;
	}

	return false;
}

void FileParser::ReflectGetFunctionParameters(FileParsedData& fileData)
{
	fileData.Cursor = static_cast<int>(fileData.Data.find('(', fileData.Cursor));
	++fileData.Cursor;

	ReflectFunctionData& funcData = fileData.ReflectData.back().Functions.back();

	while (fileData.Data[fileData.Cursor] != ')')
	{
		auto [type, name] = ReflectTypeAndName(fileData, { ',', ')' });
		funcData.Parameters.push_back(
			{
				type, 
				name, 
				DEFAULT_TYPE_SIZE
			});
	}
}

std::tuple<std::string, std::string> FileParser::ReflectTypeAndName(FileParsedData& fileData, const std::vector<char>& endOfLineCharacters)
{
	std::string type;
	bool typeFound = false;
	std::string name;
	bool nameFound = false;

	while (true)
	{
		char c = fileData.Data[fileData.Cursor];
		if (c == '}')
		{
			// Break here if we have finished
			break;
		}

		if (RefectCheckForEndOfLine(fileData) || (std::find(endOfLineCharacters.begin(), endOfLineCharacters.end(), c) != endOfLineCharacters.end()))
		{
			if (ReflectTypeCheck(type))
			{
				type += ' ';
				++fileData.Cursor;
				continue;
			}

			if (!typeFound)
			{
				if (!type.empty())
				{
					typeFound = true;
				}
			}
			else if (!nameFound)
			{
				if (!name.empty())
				{
					nameFound = true;
				}
			}
		}
		else if (c != '\n' && c != '\t') /*if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_')*/
		{
			if (!typeFound)
			{
				type += c;
			}
			else if (!nameFound)
			{
				name += c;
			}
		}

		if ((typeFound && nameFound))
		{
			break;
		}
		++fileData.Cursor;
	}

	return std::make_tuple<std::string, std::string>(type.c_str(), name.c_str());
}

NAMESPACE_END