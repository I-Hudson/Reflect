#include "FileParser.h"
#include <sstream>
#include <vector>
#include <iostream>
#include <filesystem>
#include <stack>

NAMESPACE_START

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
			CheckExtension(filePath, {".h", ".hpp"}))
		{
			// TODO thread this. We could load files on more than one thread to speed
			// this up.
			std::cout << f.file_size() << std::endl;
			std::ifstream file = OpenFile(filePath);
			FileParsedData data = LoadFile(file);
			m_filesParsed[filePath] = data;
			CloseFile(file);
		}
	}


	// All files have been loaded.
	// Now we need to parse them to find all the information we want from them.
	// TODO this could also be threaded.
	for (const auto& kvp : m_filesParsed)
	{
		ParseFile(m_filesParsed.at(kvp.first));
	}
}

std::ifstream FileParser::OpenFile(const std::string& filePath)
{
	std::ifstream file = std::ifstream(filePath, std::ios::in);
	if (!file.is_open())
	{
		//todo assert.
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
	if (!ReflectContainer(fileData, RefectStructKey, ReflectType::Struct) && !ReflectContainer(fileData, RefectClassKey, ReflectType::Class))
	{
		return;
	}

	// Good, we have a reflected container class/struct.
	// First find out which it is and verify that we are inheriting from "ReflectObject".
	std::stack<char> bracketStack;
	bool findStart = true;

	std::string type;
	bool typeFound = false;
	std::string name;
	bool nameFound = false;

	auto reset = [&type, &typeFound, &name, &nameFound]()
	{
		type = "";
		typeFound = false;
		name = "";
		nameFound = false;
	};

	while (true)
	{
		char c = fileData.Data[fileData.Cursor];
		if (findStart)
		{
			if (c == '{')
			{
				bracketStack.push('{');
				findStart = false;
			}
		}
		else
		{
			if (c == '{')
			{
				bracketStack.push('{');
			}
			else if (c == '}')
			{
				bracketStack.pop();
			}

			if (bracketStack.empty())
			{
				// Class/Struct has finished
				break;
			}

			if (RefectCheckForEndOfLine(fileData))
			{
				if (ReflectTypeCheck(type))
				{
					type += ' ';
					++fileData.Cursor;
					continue;
				}

				if (!typeFound)
				{
					typeFound = true;
				}
				else if (!nameFound)
				{
					nameFound = true;
				}

				// if c == '(' start of a function. Call a new function to handle this.

				if (c == ';')
				{
					// We have found a member variable 
					fileData.ReflectData.Members.push_back(
						{
							type, 
							name,
							0
						});

					reset();
				}
			}
			else if(c != '\n' && c != '\t') /*if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_')*/
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
		}
		++fileData.Cursor;
	}
}

bool FileParser::ReflectContainer(FileParsedData& fileData, const std::string& keyword, const ReflectType type)
{
	// Check if we can reflect this class/struct. 
	int reflectStart = static_cast<int>(fileData.Data.find(keyword));
	if (reflectStart == std::string::npos)
	{
		// Can't reflect this class. Return.
		return false;
	}

	fileData.ReflectData.Type = type;
	fileData.Cursor = reflectStart + keyword.length() + 1;
	
	// Get the flags passed though the REFLECT macro.
	std::string flag;
	bool foundContainerName = false;

	while (fileData.Data[fileData.Cursor] != ')')
	{
		if (fileData.Data[fileData.Cursor] == ',')
		{
			if (!flag.empty())
			{
				if (!foundContainerName)
				{
					foundContainerName = true;
					fileData.ReflectData.Name = flag;
				}
				else
				{
					fileData.ReflectData.ContainerProps.push_back(
						StringToReflectContainerProps(flag));
				}
			}
			flag = "";
		}
		else
		{
			if (fileData.Data[fileData.Cursor] != ' ')
			{
				flag += fileData.Data[fileData.Cursor];
			}
		}
		++fileData.Cursor;
	}
	++fileData.Cursor;

	if (!foundContainerName)
	{
		foundContainerName = true;
		fileData.ReflectData.Name = flag;
	}
	else
	{
		if (!flag.empty())
		{
			fileData.ReflectData.ContainerProps.push_back(
				StringToReflectContainerProps(flag));
		}
	}
	return true;
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

NAMESPACE_END