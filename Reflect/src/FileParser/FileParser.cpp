#include "File Parser.h"
#include <sstream>
#include <vector>
#include <iostream>

#if _WIN32
#if _HAS_CXX17
#include <filesystem>
#else
/*
 * Include windows.h without Windows Sockets 1.1 to prevent conflicts with
 * Windows Sockets 2.0.
 */
#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#endif
#endif

NAMESPACE_START

FileParser::FileParser()
{ }

FileParser::~FileParser()
{ }

void FileParser::ParseDirectory(const std::string& dir)
{
#if _WIN32
#if _HAS_CXX17
	for (const auto& f : std::filesystem::recursive_directory_iterator(dir))
	{
		std::string filePath = f.path().u8string();
		std::cout << filePath << std::endl;

		if ((f.is_regular_file() || f.is_character_file()) && 
			CheckExtension(filePath, {".h", ".hpp"}))
		{
			std::cout << f.file_size() << std::endl;
			std::ifstream file = OpenFile(filePath);
			FileParsedData data = LoadFile(file);
			m_filesParsed[filePath] = data;
			CloseFile(file);
		}
	}
#else 
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile((LPCWSTR)dir.c_str(), &FindFileData);
	if(hFind == INVALID_HANDLE_VALUE) 
	{
		printf("FindFirstFile failed (%d)\n", GetLastError());
		return;
	}

	do 
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
		{
			std::wstring ws(FindFileData.cFileName);
			// your new String
			std::string filePath(ws.begin(), ws.end());

			std::ifstream file = OpenFile(filePath);
			FileParsedData data = LoadFile(file);
			m_filesParsed[filePath] = data;
			CloseFile(file);
		}
	} while (FindNextFile(hFind, &FindFileData) != 0);
	FindClose(hFind);
#endif
#endif
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
	int fileSize = file.tellg();
	file.seekg(0, std::ios::beg);
	data.Data = std::string(fileSize, '\0');
	data.Cursor = 0;
	file.read(&data.Data[0], fileSize);

	return std::move(data);
}

void FileParser::ParseFile(std::ifstream& file)
{

}

void FileParser::ParseLine(const std::string& line)
{
	ExtractReflectType(line);
	ExtractReturnType(line);
	ExtractName(line);
	ExtractParameters(line);
}

void FileParser::ExtractReflectType(const std::string& line)
{
}

void FileParser::ExtractReturnType(const std::string& line)
{
}

void FileParser::ExtractName(const std::string& line)
{
}

void FileParser::ExtractParameters(const std::string& line)
{
}

NAMESPACE_END