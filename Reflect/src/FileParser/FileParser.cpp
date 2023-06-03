#include "FileParser/FileParser.h"
#include "FileParser/FileParserKeyWords.h"

#include "Core/Core.h"
#include "Core/Util.h"
#include "Instrumentor.h"

#include <sstream>
#include <vector>
#include <iostream>
#include <filesystem>
#include <stack>
#include <assert.h>
#include <string.h>

#define EXP_PARSER

namespace Reflect::Parser
{
	constexpr int DEFAULT_TYPE_SIZE = 0;

	const std::vector<char> emptyChars = { '\n', '\t', '\r', ' ' };
	const std::vector<char> generalEndChars = { ' ', '(', '=', ';', ':'};
	const std::vector<char> functionStartChars = { '(' };
	const std::vector<char> memberStartChars = { '=', ';' };

	FileParser::FileParser()
	{ }

	FileParser::~FileParser()
	{ }

	void FileParser::ParseDirectory(const std::string& directory, const ReflectAddtionalOptions* options)
	{
		REFLECT_PROFILE_FUNCTION();

		m_filesParsed.clear();
		m_filesToRemove.clear();
		m_options = options;

		m_directoriesParsed.push_back(directory);
		std::filesystem::path dirPath(directory);
		std::error_code err;
		if (!std::filesystem::is_directory(dirPath, err))
		{
			std::cout << err.message() << '\n';
			return;
		}

		for (const auto& f : std::filesystem::recursive_directory_iterator(directory))
		{
			std::string filePath = f.path().u8string();

			if ((f.is_regular_file() || f.is_character_file()) &&
				CheckExtension(filePath, { ".h", ".hpp" }) &&
				!CheckIfAutoGeneratedFile(filePath))
			{
				// TODO thread this. We could load files on more than one thread to speed
				// this up.
				std::cout << "Parsing: " << filePath << std::endl;
				std::ifstream file = OpenFile(filePath);
				FileParsedData data = LoadFile(file);
				data.FileName = f.path().filename().u8string().substr(0, f.path().filename().u8string().find_last_of('.'));
				data.FilePath = f.path().parent_path().u8string();
				m_filesParsed.push_back(data);
				CloseFile(file);
			}
		}


		// All files have been loaded.
		// Now we need to parse them to find all the information we want from them.
		// TODO: this could also be threaded.
		for (auto& file : m_filesParsed)
		{
			if (!ParseFile(file))
			{
				m_filesToRemove.push_back(file.FileName);
			}
		}

		for (auto& file : m_filesParsed)
		{
			for (auto& reflectedData : file.ReflectData)
			{
				for (auto& inheritanceItem : reflectedData.Inheritance)
				{
					// This is not great. Currently the solution is to try and find a reflected container with the same name 
					// regardless of the namespace. So if you have two classes named the same in different namespaces this won't work.
					if (Parser::ReflectContainerData* inheritanceData = FindReflectContainerData(inheritanceItem.Name); 
						inheritanceData != nullptr)
					{
						inheritanceItem.IsReflected = true;
						inheritanceItem.NameWithNamespace = inheritanceData->NameWithNamespace;
					}
					else
					{
						inheritanceItem.IsReflected = false;
					}
				}
			}
		}

		for (auto const& fileToRemove : m_filesToRemove)
		{
			auto itr = std::find_if(m_filesParsed.begin(), m_filesParsed.end(), [fileToRemove](FileParsedData const& data)
			{
				return fileToRemove == data.FileName;
			});
			assert(itr != m_filesParsed.end() && "[FileParser::ParseDirectory] Remove file to parse dose not exists.");
			m_filesParsed.erase(itr);
		}
	}

	void FileParser::SetIgnoreStrings(const std::vector<std::string>& ignoreStrings)
	{
		REFLECT_PROFILE_FUNCTION();
		m_ignoreStrings = ignoreStrings;
	}

	std::ifstream FileParser::OpenFile(const std::string& filePath)
	{
		REFLECT_PROFILE_FUNCTION();
		std::ifstream file = std::ifstream(filePath);
		assert(file.is_open() && "[FileParser::OpenFile] File is not open.");
		return file;
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
		REFLECT_PROFILE_FUNCTION();
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

	bool FileParser::CheckIfAutoGeneratedFile(const std::string& filePath)
	{
		REFLECT_PROFILE_FUNCTION();
		return filePath.find(Keys::ReflectFileGeneratePrefix) != std::string::npos;
	}

	FileParsedData FileParser::LoadFile(std::ifstream& file)
	{
		REFLECT_PROFILE_FUNCTION();
		FileParsedData data = {};

		file.seekg(0, std::ios::end);
		int fileSize = static_cast<int>(file.tellg());
		file.seekg(0, std::ios::beg);
		data.Data = std::string(fileSize, '\0');
		data.Cursor = 0;
		file.read(&data.Data[0], fileSize);

		return data;
	}

	bool FileParser::ParseFile(FileParsedData& fileData)
	{
		REFLECT_PROFILE_FUNCTION();

		bool reflectItem = false;
		while (ReflectContainerHeader(fileData, Keys::RefectStructKey, EReflectType::Struct) 
			|| ReflectContainerHeader(fileData, Keys::RefectClassKey, EReflectType::Class))
		{
			ReflectContainer(fileData);
			GetAllCPPIncludes(fileData);
			reflectItem = true;
		}
		return reflectItem;
	}

	bool FileParser::ReflectContainerHeader(FileParsedData& fileData, const std::string& keyword, const EReflectType type)
	{
		REFLECT_PROFILE_FUNCTION();

		// Check if we can reflect this class/struct. 
		int reflectStart = static_cast<int>(fileData.Data.find(keyword, fileData.Cursor));
		if (reflectStart == std::string::npos)
		{
			// Can't reflect this class/struct. Return.
			return false;
		}
		Parser::ReflectContainerData containerData = {};

		containerData.Namespaces = FindAllNamespaces(fileData, reflectStart);

		containerData.ReflectType = type;
		fileData.Cursor = reflectStart + static_cast<int>(keyword.length()) + 1;
		containerData.ContainerProps = ReflectFlags(fileData);

		if (containerData.ReflectType == EReflectType::Class)
		{
			int newPos = (int)fileData.Data.find("class", fileData.Cursor);
			if (newPos != std::string::npos)
			{
				fileData.Cursor = newPos;
				fileData.Cursor += 5;
			}
			else
			{
				return false;
			}
		}
		else if (containerData.ReflectType == EReflectType::Struct)
		{
			int newPos = (int)fileData.Data.find("struct", fileData.Cursor);
			if (newPos != std::string::npos)
			{
				fileData.Cursor = newPos;
				fileData.Cursor += 6;
			}
			else
			{
				return false;
			}
		}

		// Get the flags passed though the REFLECT macro.
		std::string containerName;
		while (fileData.Data.at(fileData.Cursor) != ':' && fileData.Data.at(fileData.Cursor) != '{' && fileData.Data.at(fileData.Cursor) != '\n')
		{
			if (fileData.Data.at(fileData.Cursor) != ' ')
			{
				containerName += fileData.Data.at(fileData.Cursor);
			}
			++fileData.Cursor;
		}
		for (const std::string str : m_ignoreStrings)
			Util::RemoveString(containerName, str);

		containerData.Name = containerName;
		containerData.PrettyName = PrettyString(containerName);
		containerData.Type = containerName;
		containerData.TypeSize = DEFAULT_TYPE_SIZE;

		// We are inheriting things.
		bool inheritance = fileData.Data.find(':', fileData.Cursor) < fileData.Data.find('{', fileData.Cursor);
		if (inheritance)
		{
			FindNextChar(fileData, ':');
			++fileData.Cursor;
			std::string type;
			while (fileData.Data.at(fileData.Cursor) != '{')
			{
				if (fileData.Data.at(fileData.Cursor) == ',')
				{
					Util::RemoveCharAll(type, ' ');
					Util::RemoveString(type, Keys::PublicKey);
					Util::RemoveString(type, Keys::ProtectedKey);
					Util::RemoveString(type, Keys::PrivateKey);
					containerData.Inheritance.push_back({ type });
					type.clear();
				}
				else
				{
					type += fileData.Data.at(fileData.Cursor);
				}
				++fileData.Cursor;
			}
			Util::RemoveCharAll(type, ' ');
			Util::RemoveCharAll(type, '\n');
			Util::RemoveCharAll(type, '\t');
			Util::RemoveCharAll(type, '\r');
			Util::RemoveString(type, Keys::PublicKey);
			Util::RemoveString(type, Keys::ProtectedKey);
			Util::RemoveString(type, Keys::PrivateKey);

			while (type.find("::") != std::string::npos)
			{
				uint64_t index = type.find("::");
				type = type.substr(index + 2);
			}
			containerData.Inheritance.push_back({ type });
		}

		if (!containerData.Namespaces.empty())
		{
			for (const std::string& str : containerData.Namespaces)
			{
				containerData.NameWithNamespace += str + "::";
			}
		}
		containerData.NameWithNamespace += containerData.Name;

		fileData.ReflectData.push_back(containerData);
		return true;
	}

	void FileParser::ReflectContainer(FileParsedData& fileData)
	{
		REFLECT_PROFILE_FUNCTION();

		int endOfContainerCursor = FindEndOfConatiner(fileData);

		// Good, we have a reflected container class/struct.
		// First find out which it is and verify that we are inheriting from "ReflectObject".
		std::stack<char> bracketStack;
		Parser::ReflectContainerData& conatinerData = fileData.ReflectData.back();

		int generatedBodyLine = static_cast<int>(fileData.Data.find(Keys::ReflectGeneratedBodykey, fileData.GeneratedBodyLineOffset));
		std::cout << "[FileParser::ReflectContainer] 'REFLECT_GENERATED_BODY()' is missing from a container.";
		//assert(generatedBodyLine != -1 && );
		fileData.GeneratedBodyLineOffset = generatedBodyLine + static_cast<int>(strlen(Keys::ReflectGeneratedBodykey));
		conatinerData.ReflectGenerateBodyLine = CountNumberOfSinceTop(fileData, generatedBodyLine, '\n') + 1;

		// Set us to the start of the class/struct. We should continue until we find something.
		char c = FindNextChar(fileData, '{');
		std::vector<std::string> reflectFlags;
		while (true)
		{
			if (CheckForEndOfFile(fileData, endOfContainerCursor))
				break;

			if (m_options->options.at(Keys::Reflect_CMD_Option_Reflect_Full_EXT) == "true")
			{
				c = FindNextChar(fileData, emptyChars);
				std::string word = FindNextWord(fileData, generalEndChars);

				if (CheckForTypeAlias(word))
				{
					FindNextChar(fileData, ';');
					continue;
				}

				// Check for child strcuts/class

				if (CheckForVisibility(word))
				{
					++fileData.Cursor;
					continue;
				}

				// Check for constructor/destructor
				if (CheckForConstructor(fileData, conatinerData, word))
					continue;

				if (CheckForIgnoreWord(fileData, word))
					continue;

				if (CheckForOperatorFunction(fileData, word))
					continue;

				if (CheckForComments(fileData, word))
					continue;

				if (CheckForFriends(fileData, word))
					continue;

				// We should now have a type (unless there are macros or compiler keywords).
				if (IsWordReflectKey(word))
				{
					if (word == Keys::ReflectGeneratedBodykey)
					{
						c = FindNextChar(fileData, { '(', ')' });
						continue;
					}
					else if (word == Keys::ReflectPropertyKey)
					{
						// Get the flags for the property
						reflectFlags = ReflectFlags(fileData);
						continue;
					}
				}
				else
				{
					// We think we have function or memeber.
					fileData.Cursor -= (int)word.size();
				}

				if (CheckForEndOfFile(fileData, endOfContainerCursor))
					break;

				EReflectType refectType = CheckForReflectType(fileData);
				if (refectType == EReflectType::Member)
				{
					conatinerData.Members.push_back(GetMember(fileData, reflectFlags));
				}
				else if (refectType == EReflectType::Function)
				{
					if (!CheckForConstructor(fileData, conatinerData, word))
					{
						Parser::ReflectFunctionData funcData = GetFunction(fileData, reflectFlags);
						if (!funcData.Name.empty())
						{
							conatinerData.Functions.push_back(funcData);
						}
					}
				}
				//else
				//{
				//	assert(false);
				//	continue;
				//}
			}
			else
			{
				if (CheckForEndOfFile(fileData, endOfContainerCursor))
					break;

				int newCursor = (int)fileData.Data.find(Keys::ReflectPropertyKey, fileData.Cursor);
				if (newCursor > endOfContainerCursor)
				{
					fileData.Cursor = endOfContainerCursor;
					break;
				}
				fileData.Cursor = newCursor;

				if (fileData.Cursor == std::string::npos)
				{
					break;
				}

				fileData.Cursor += (int)strlen(Keys::ReflectPropertyKey);
				reflectFlags = ReflectFlags(fileData);
				FindNextChar(fileData, generalEndChars);


				EReflectType refectType = CheckForReflectType(fileData);
				if (refectType == EReflectType::Member)
				{
					conatinerData.Members.push_back(GetMember(fileData, reflectFlags));
				}
				else if (refectType == EReflectType::Function)
				{
					conatinerData.Functions.push_back(GetFunction(fileData, reflectFlags));
				}
			}

			reflectFlags = {};
		}
	}

	void FileParser::GetAllCPPIncludes(FileParsedData& fileData)
	{
		REFLECT_PROFILE_FUNCTION();

		size_t index = fileData.Data.find(Keys::ReflectCPPIncludeKey);
		while (index != std::string::npos)
		{
			std::string line = fileData.Data.substr(index, fileData.Data.find(';', index) - index);
			line = line.substr(line.find('(') + 2, line.find(')') - (line.find('(') + 3));
			fileData.CPPIncludes.push_back(line);
			index = fileData.Data.find(Keys::ReflectCPPIncludeKey, fileData.Data.find(';', index));
		}
	}

	std::vector<std::string> FileParser::FindAllNamespaces(FileParsedData fileData, int reflectStart)
	{
		fileData.Cursor = reflectStart;
		std::vector<std::string> namespaces;
		int closeBracketCount = 0;

		while (fileData.Cursor != 0)
		{
			if (fileData.Data[fileData.Cursor] == '}')
			{
				++closeBracketCount;
			}
			else if (fileData.Data[fileData.Cursor] == '{')
			{
				if (closeBracketCount == 0)
				{
					--fileData.Cursor;
					std::string namespaceName = FindPreviousNamespace(fileData);
					if (!namespaceName.empty())
					{
						namespaces.push_back(namespaceName);
					}
				}
				else
				{
					--closeBracketCount;
				}
			}
			--fileData.Cursor;
		}
	
		if (!namespaces.empty())
		{
			std::reverse(namespaces.begin(), namespaces.end());
		}

		return namespaces;
	}

	std::string FileParser::FindPreviousNamespace(FileParsedData& fileData)
	{
		FindNextChar(fileData, emptyChars, true);
		std::string namespaceName = FindNextWord(fileData, emptyChars, true);

		FindNextChar(fileData, emptyChars, true);
		std::string namespaceAlias = FindNextWord(fileData, emptyChars, true);

		if (namespaceAlias == Keys::Namespace)
		{
			return namespaceName;
		}
		return "";
	}

	int FileParser::FindEndOfConatiner(const FileParsedData& fileData)
	{
		REFLECT_PROFILE_FUNCTION();

		int cursor = fileData.Cursor;
		char lastCharacter = '\0';
		char c = fileData.Data[cursor];
		bool foundStartOfContainer = false;
		std::stack<char> symbols;
		while (true)
		{
#ifdef EXP_PARSER
			if (!foundStartOfContainer)
			{
				if (c == '{')
				{
					symbols.push(c);
					foundStartOfContainer = true;
				}
			}
			else
			{
				if (c == '{')
				{
					symbols.push(c);
				}
				else if (c == '}')
				{
					symbols.pop();
				}

				if (symbols.size() == 0)
				{
					char nextChar = FindNextChar(fileData, cursor, emptyChars);
					assert(nextChar == ';' && "[FileParser::FindEndOfConatiner] Did not find ';' at end of the container.");
					break;
				}
			}
#else
			if (lastCharacter == '}' && c == ';')
			{
				break;
			}

			if (c != '\t' && c != '\n')
			{
				lastCharacter = c;
			}
#endif
			++cursor;
			c = fileData.Data[cursor];
		}
		return cursor;
	}

	std::vector<std::string> FileParser::ReflectFlags(FileParsedData& fileData)
	{
		REFLECT_PROFILE_FUNCTION();

		// Get the flags passed though the REFLECT macro.
		std::string flag;
		std::vector<std::string> flags;

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
					flags.push_back(flag);
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
			flags.push_back(flag);
		}

		return flags;
	}

	char FileParser::FindNextChar(FileParsedData const& fileData, int& cursor, const std::vector<char>& ignoreChars, bool reverse)
	{
		REFLECT_PROFILE_FUNCTION();

		FileParsedData copyFileData = fileData;
		copyFileData.Cursor = cursor;
		char c = FindNextChar(copyFileData, ignoreChars, reverse);
		cursor = copyFileData.Cursor;
		return c;
	}

	char FileParser::FindNextChar(FileParsedData& fileData, char charToFind, bool reverse)
	{
		REFLECT_PROFILE_FUNCTION();

		char c = fileData.Data[fileData.Cursor];
		while (c != charToFind)
		{
			if (reverse ? --fileData.Cursor : ++fileData.Cursor < fileData.Data.size())
				c = fileData.Data[fileData.Cursor];
			else
				break;
		}
		return c;
	}

	char FileParser::FindNextChar(FileParsedData& fileData, const std::vector<char>& ignoreChars, bool reverse)
	{
		REFLECT_PROFILE_FUNCTION();

		reverse ? --fileData.Cursor : ++fileData.Cursor;
		while (std::find(ignoreChars.begin(), ignoreChars.end(), fileData.Data[fileData.Cursor]) != ignoreChars.end())
		{
			reverse ? --fileData.Cursor : ++fileData.Cursor;
		}
		return fileData.Data[fileData.Cursor];
	}

	std::string FileParser::FindNextWord(FileParsedData& fileData, const std::vector<char>& endChars, bool reverse)
	{
		REFLECT_PROFILE_FUNCTION();

		std::string s;
		s += fileData.Data[fileData.Cursor];
		char c = FindNextChar(fileData, std::vector<char>(), reverse);
		while (std::find(endChars.begin(), endChars.end(), c) == endChars.end())
		{
			s += c;
			c = FindNextChar(fileData, std::vector<char>(), reverse);
		}

		if (reverse)
		{
			std::reverse(s.begin(), s.end());
		}

		return s;
	}

	std::string FileParser::FindNextWord(const FileParsedData& fileData, int& cursor, const std::vector<char>& endChars, bool reverse)
	{
		FileParsedData copyFileData = fileData;
		copyFileData.Cursor = cursor;
		return FindNextWord(copyFileData, endChars, reverse);
	}

	bool FileParser::IsWordReflectKey(std::string_view view)
	{
		REFLECT_PROFILE_FUNCTION();
		return view == Keys::ReflectGeneratedBodykey 
			|| view == Keys::ReflectPropertyKey;
	}

	bool FileParser::CheckForTypeAlias(std::string_view view)
	{
		REFLECT_PROFILE_FUNCTION();
		return view == Keys::TypedefKey 
			|| view == Keys::UsingKey;
	}

	bool FileParser::CheckForVisibility(std::string_view view)
	{
		REFLECT_PROFILE_FUNCTION();
		return view == Keys::PublicKey 
			|| view == Keys::ProtectedKey 
			|| view == Keys::PrivateKey;
	}

	bool FileParser::CheckForConstructor(FileParsedData& fileData, Parser::ReflectContainerData& container, std::string_view view)
	{
		REFLECT_PROFILE_FUNCTION();

		int i;
		FileParsedData copy = fileData;
		std::string line = GetFunctionLine(copy, i);
		if (line.find('~') != std::string::npos)
		{
			SkipFunctionBody(fileData);
			return true;
		}

		if (view == container.Name || view.at(0) == '~')
		{
			SkipFunctionBody(fileData);
			return true;
		} 
		return false;
	}

	bool FileParser::CheckForIgnoreWord(FileParsedData& fileData, std::string_view view)
	{
		REFLECT_PROFILE_FUNCTION();

		for (const std::string& str : m_ignoreStrings)
		{
			if (view == str)
			{
				FindNextChar(fileData, ';');
				return true;
			}
		}
		return false;
	}

	bool FileParser::CheckForOperatorFunction(FileParsedData& fileData, std::string_view view)
	{
		REFLECT_PROFILE_FUNCTION();

		int i;
		std::string line = GetFunctionLine(fileData, i);
		size_t index = line.find(Keys::OperatorKey);
		if (index != std::string::npos)
		{
			SkipFunctionBody(fileData);
			return true;
		}
		return false;
	}

	bool FileParser::CheckForComments(FileParsedData& fileData, std::string& line)
	{
		REFLECT_PROFILE_FUNCTION();

		// Remove all contents of a line with comments.
		size_t index = line.find("//");
		if (index != std::string::npos)
		{
			line = line.substr(0, index);
			FindNextChar(fileData, '\n');
			return true;
		}
		return false;
	}

	bool FileParser::CheckForFriends(FileParsedData& fileData, std::string_view view)
	{
		REFLECT_PROFILE_FUNCTION();

		if (view == Keys::FriendKey)
		{
			FindNextChar(fileData, ';');
			return true;
		}
		return false;
	}

	void FileParser::GetReflectNameAndReflectValueTypeAndReflectModifer(std::string& str, std::string& name, EReflectValueType& valueType, EReflectValueModifier& modifer)
	{
		REFLECT_PROFILE_FUNCTION();

		name = Util::Reverse(name);
		Util::RemoveCharAll(name, ' ');
		Util::RemoveString(str, name);

		modifer = CheckForMemberModifers(str);
		Util::RemoveString(str, Keys::ConstKey);
		Util::RemoveString(str, Keys::StaticKey);
		Util::RemoveString(str, Keys::VolatileKey);
		Util::RemoveString(str, Keys::VirtualKey);

		if (str.find('<') == std::string::npos && str.find('>') == std::string::npos)
		{
			valueType = CheckForRefOrPtr(str);
			Util::RemoveCharAll(str, '&');
			Util::RemoveCharAll(str, '*');
		}
		else
		{
			Util::RemoveCharAll(str, ' ');
			std::string stdContainerRefPtr = str.substr(str.find_last_of('>'));
			valueType = CheckForRefOrPtr(stdContainerRefPtr);
		}
	}

	Parser::ReflectFunctionData FileParser::GetFunction(FileParsedData& fileData, const std::vector<std::string>& flags)
	{
		REFLECT_PROFILE_FUNCTION();

		Parser::ReflectFunctionData functionData;

		int endOfLineCursor;
		std::string line = GetFunctionLine(fileData, endOfLineCursor);

		uint32_t cBracket = (uint32_t)line.find_last_of(')');
		size_t functionConst = (uint32_t)line.find(Keys::ConstKey, cBracket);
		if (functionConst != std::string::npos)
		{
			functionData.IsConst = true;
			line = line.substr(0, cBracket + 1);
		}
		uint32_t oBracket = (uint32_t)line.find_first_of('(');
		std::string prameters = line.substr(oBracket, cBracket);
		Util::RemoveString(line, prameters);
		// Parse the parameters.
		functionData.Parameters = ReflectGetFunctionParameters(fileData, prameters);

		// Make sure if there are any spaces between the parameters and the function name we check for them.
		uint32_t cursor = (uint32_t)line.size() - 1;
		while (std::find(emptyChars.begin(), emptyChars.end(), line.at(cursor)) != emptyChars.end())
			--cursor;

		while (std::find(emptyChars.begin(), emptyChars.end(), line.at(cursor)) == emptyChars.end())
		{
			functionData.Name += line.at(cursor);
			--cursor;
		}
		// We should now have just the type.
		// TODO: Think about how to handle 'inline' modifiers and suck.
		// TODO: template support?
		GetReflectNameAndReflectValueTypeAndReflectModifer(line, functionData.Name, functionData.ReflectValueType, functionData.ReflectModifier);

		// Make sure there are no empty chars in the type string.
		for (const char& c : emptyChars)
			Util::RemoveCharAll(line, c);
		functionData.Type = line;

		fileData.Cursor = endOfLineCursor;
		return functionData;
	}

	ReflectMemberData FileParser::GetMember(FileParsedData& fileData, const std::vector<std::string>& flags)
	{
		REFLECT_PROFILE_FUNCTION();

		ReflectMemberData memberData;
		memberData.TypeSize = DEFAULT_TYPE_SIZE;
		memberData.ContainerProps = flags;

		FileParsedData copy = fileData;
		FindNextChar(copy, ';');
		std::string line = fileData.Data.substr(fileData.Cursor, copy.Cursor - fileData.Cursor);
		int endOfLineCursor = fileData.Cursor + (int)line.size();
		line += ';';

		// Check for if there is a deault value being set.
		uint32_t equalCursor = (uint32_t)line.find('=');
		uint32_t semicolonCursor = (uint32_t)line.find(';');
		uint32_t cursor = equalCursor < semicolonCursor ? equalCursor : semicolonCursor;

		// Always go back one so we are not on '=' or ';';
		--cursor;
		// Go back untill we are not on an empty char.
		while (std::find(emptyChars.begin(), emptyChars.end(), line.at(cursor)) != emptyChars.end())
			--cursor;
		line = line.substr(0, cursor + 1);

		// 'line' should now contain the value, member name and any modifers like const.
		std::vector<char> endChars = emptyChars;
		endChars.push_back('*');
		endChars.push_back('&');
		while (std::find(endChars.begin(), endChars.end(), line.at(cursor)) == endChars.end())
		{
			memberData.Name += line.at(cursor);
			--cursor;
		}
		GetReflectNameAndReflectValueTypeAndReflectModifer(line, memberData.Name, memberData.ReflectValueType, memberData.ReflectModifier);

		cursor = (uint32_t)line.size() - 1;
		while (std::find(emptyChars.begin(), emptyChars.end(), line.at(cursor)) != emptyChars.end())
			--cursor;
		std::string type = line.substr(0, cursor + 1);

		// Make sure there are no empty chars in the type string.
		for (const char& c : emptyChars)
			Util::RemoveCharAll(type, c);
		memberData.Type = type;

		memberData.RawType = type;

		fileData.Cursor = endOfLineCursor;
		return memberData;
	}

	void FileParser::SkipFunctionBody(FileParsedData& fileData)
	{
		REFLECT_PROFILE_FUNCTION();

		FileParsedData bracketCursor = fileData;
		FindNextChar(bracketCursor, '{');
		FileParsedData semicolonCursor = fileData;
		FindNextChar(semicolonCursor, ';');

		if (semicolonCursor.Cursor < bracketCursor.Cursor)
		{
			fileData.Cursor = semicolonCursor.Cursor;
			return;
		}
		fileData.Cursor = bracketCursor.Cursor;

		std::stack<char> brackets;
		char c = fileData.Data[fileData.Cursor];
		while (true)
		{
			if (c == '{')
				brackets.push('{');
			else if (c == '}')
				brackets.pop();

			if (brackets.size() == 0)
			{
				break;
			}
			c = fileData.Data[++fileData.Cursor];
		}
	}

	EReflectType FileParser::CheckForReflectType(FileParsedData& data)
	{
		REFLECT_PROFILE_FUNCTION();

		auto find_closest_char = [data, this](std::vector<char> const& chars_to_find)
		{
			int cursor = INT_MAX;
			for (size_t i = 0; i < chars_to_find.size(); ++i)
			{
				char charToFind = chars_to_find.at(i);
				FileParsedData copy = data;
				FindNextChar(copy, charToFind);
				cursor = std::min(copy.Cursor, cursor);
			}
			return cursor;
		};

		int member_cursor = find_closest_char(memberStartChars);
		int function_cursor = find_closest_char(functionStartChars);

		bool isTemplate = data.Data.find(Keys::TemplateKey, data.Cursor) < member_cursor;

		if (member_cursor < function_cursor && !isTemplate)
		{
			return EReflectType::Member;
		}
		else if (function_cursor != INT_MAX && !isTemplate)
		{
			return EReflectType::Function;
		}

		if (isTemplate)
		{
			SkipFunctionBody(data);
		}
		return EReflectType::Unknown;
	}

	bool FileParser::CheckForEndOfFile(FileParsedData& fileData, int cursor)
	{
		REFLECT_PROFILE_FUNCTION();

		if (fileData.Cursor >= cursor)
		{
			return true;
		}

		FileParsedData copy = fileData;
		char c = copy.Data[copy.Cursor];
		bool endOfFile = false;
		char previousValidChar = c;
		while (copy.Cursor < cursor)
		{
			++copy.Cursor;
			c = copy.Data[copy.Cursor];
			if (std::find(emptyChars.begin(), emptyChars.end(), c) == emptyChars.end())
			{
				if (previousValidChar == '}')
				{
					endOfFile = c == ';';
				}
				break;
			}
		}
		if (endOfFile)
			fileData.Cursor = ++copy.Cursor;
		return endOfFile || copy.Cursor == cursor;
	}

	EReflectValueType FileParser::CheckForRefOrPtr(std::string_view view)
	{
		REFLECT_PROFILE_FUNCTION();

		size_t referenceIndex = view.find(Keys::ReferenceKey);
		size_t pointerIndex = view.find(Keys::PointerKey);

		// Get the type. We need this as the code generation will need to add some casting 
		// if the type is not a value.
		if (referenceIndex == std::string::npos && pointerIndex == std::string::npos)
			return EReflectValueType::Value;
		else if (referenceIndex < pointerIndex)
			return EReflectValueType::Reference;
		else if (pointerIndex < referenceIndex && referenceIndex == std::string::npos)
			return EReflectValueType::Pointer;
		else if (pointerIndex < referenceIndex && referenceIndex != std::string::npos)
			return EReflectValueType::PointerReference;
		// TODO: Think about pointer, pointer, pointer, etc. 
		assert(false && "[FileParser::CheckForRefOrPtr] Unknow type.");
		return EReflectValueType::Value;
	}

	EReflectValueModifier FileParser::CheckForMemberModifers(std::string_view view)
	{
		REFLECT_PROFILE_FUNCTION();

		size_t constIndex = view.find(Keys::ConstKey);
		size_t staticIndex = view.find(Keys::StaticKey);
		size_t volatileIndex = view.find(Keys::VolatileKey);
		size_t virtualIndex = view.find(Keys::VirtualKey);

		if (constIndex != std::string::npos)
			return EReflectValueModifier::Const;
		else if (staticIndex != std::string::npos)
			return EReflectValueModifier::Static;
		else if (volatileIndex != std::string::npos)
			return EReflectValueModifier::Volatile;
		else if (virtualIndex != std::string::npos)
			return EReflectValueModifier::Virtual;

		//TODO: Think about const static
		return EReflectValueModifier::None;
	}

	std::string FileParser::GetFunctionLine(const FileParsedData& fileData, int& endCursor)
	{
		REFLECT_PROFILE_FUNCTION();

		FileParsedData copy = fileData;
		FindNextChar(copy, ';');
		endCursor = copy.Cursor;
		copy = fileData;
		FindNextChar(copy, '{');
		if (copy.Cursor < endCursor)
		{
			SkipFunctionBody(copy);
			endCursor = copy.Cursor;
		}
		else
		{
			endCursor = std::min(copy.Cursor, endCursor);
		}
		return fileData.Data.substr(fileData.Cursor, endCursor - fileData.Cursor);
	}

	std::vector<Parser::ReflectTypeNameData> FileParser::ReflectGetFunctionParameters(const FileParsedData& fileData, std::string_view view)
	{
		REFLECT_PROFILE_FUNCTION();

		int cursor = 0;
		if (view.at(0) == '(')
			++cursor;

		std::vector<Parser::ReflectTypeNameData> parameters;
		std::string str;
		char c = view.at(cursor);
		while (cursor < view.size())
		{
			if (c != ',' && c != ')')
				str += c;
			else
			{
				if (str.size() > 0 && !Util::StringContains(str, emptyChars))
				{
					Parser::ReflectTypeNameData parameter;

					std::size_t defaultValueIndex = str.find('=');
					if (defaultValueIndex != std::string::npos)
					{
						cursor = (int)(((int)view.size() - 1) - (int)str.size()) + (int)defaultValueIndex;
					}

					// Extract the name, type for the parameter.
					int copyCursor = cursor - 1;
					while (std::find(emptyChars.begin(), emptyChars.end(), view.at(copyCursor)) != emptyChars.end())
						--copyCursor;

					while (std::find(emptyChars.begin(), emptyChars.end(), view.at(copyCursor)) == emptyChars.end())
					{
						parameter.Name += view.at(copyCursor);
						--copyCursor;
						CheckStringViewBounds(fileData, copyCursor, view);
					}
					GetReflectNameAndReflectValueTypeAndReflectModifer(str, parameter.Name, parameter.ReflectValueType, parameter.ReflectModifier);

					while (view.at(copyCursor) != '(' && view.at(copyCursor) != ',')
					{
						parameter.Type += view.at(copyCursor);
						--copyCursor;
						CheckStringViewBounds(fileData, copyCursor, view);
					}
					parameter.Type = Util::Reverse(parameter.Type);
					Util::RemoveCharReverse(parameter.Type, Keys::ReferenceKey);
					Util::RemoveCharReverse(parameter.Type, Keys::PointerKey);

					parameter.RawType = parameter.Type;
					Util::RemoveString(parameter.RawType, Keys::ConstKey);
					Util::RemoveCharAll(parameter.RawType, Keys::WhiteSpaceKey);

					str = {};
					parameters.push_back(parameter);
				}
				if (c == ')')
				{
					break;
				}
			}
			++cursor;
			c = view.at(cursor);
			//auto [type, name, isConst] = ReflectTypeAndName(fileData, { ',', ')' });
			//parameters.push_back(
			//	{
			//		type,
			//		name,
			//		DEFAULT_TYPE_SIZE,
			//		type.back() == '*' || type.back() == '&' ? (type.back() == '*' ? EReflectValueType::Pointer : EReflectValueType::Reference) : EReflectValueType::Value,
			//		isConst
			//	});
		}
		return parameters;
	}

	void FileParser::CheckStringViewBounds(const FileParsedData& fileData, int cursor, std::string_view view)
	{
		REFLECT_PROFILE_FUNCTION();

		if (cursor < 0 || cursor >= view.size())
		{
			Log_Error("[FileParser::CheckStringViewBounds] File: '%s', Line: '%s', Cursor: '%d'.", fileData.FileName.c_str(), view.data(), cursor);
			exit(0);
		}
	}

	int FileParser::CountNumberOfSinceTop(const FileParsedData& fileData, int cursorStart, const char& character)
	{
		REFLECT_PROFILE_FUNCTION();

		int count = 0;
		while (cursorStart > 0)
		{
			//TODO Out of bounds checks.
			if (fileData.Data[cursorStart] == character)
			{
				++count;
			}
			--cursorStart;
		}
		return count;
	}

	std::string FileParser::PrettyString(std::string str)
	{
		REFLECT_PROFILE_FUNCTION();

		std::replace(str.begin(), str.end(), '_', ' ');
		for (size_t i = 1; i < str.size(); ++i)
		{
			if (isupper(str[i]))
			{
				str.insert(str.begin() + i, ' ');
				++i;
			}
		}
		return str;
	}

	ReflectContainerData* FileParser::FindReflectContainerData(std::string_view containerName)
	{
		for (auto& file : m_filesParsed)
		{
			for (auto& reflectedData : file.ReflectData)
			{
				if (reflectedData.Name == containerName)
				{
					return &reflectedData;
				}
			}
		}
		return nullptr;
	}
}