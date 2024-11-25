#pragma once

#include "Reflect/Core/Defines.h"
#include "Reflect/Core/Options.h"
#include "Reflect/FileParser/ParserStructs.h"

#include "Reflect/Structs/PropertyMeta.h"

#include <string>
#include <fstream>
#include <unordered_map>
#include <tuple>

namespace Reflect
{
	struct ReflectAddtionalOptions;
}

namespace Reflect::Parser
{
	/// <summary>
	/// Parse a single file. This should extract all the info like functions and variables.
	/// </summary>
	class FileParser
	{
	public:
		REFLECT_API FileParser();
		REFLECT_API ~FileParser();

		REFLECT_API void ParseDirectory(const std::string& directory, const ReflectAddtionalOptions* additionalOptions, FileParserOptions fileParserOptions = { });
		/// @brief Clear all parsed file data.
		REFLECT_API void Clear();
		REFLECT_API void SetIgnoreStrings(const std::vector<std::string>& ignoreStrings);

		REFLECT_API const FileParsedData& GetFileParsedData(size_t index) const { return m_filesParsed.at(index); }
		REFLECT_API const std::vector<FileParsedData>& GetAllFileParsedData() const { return m_filesParsed; }

		REFLECT_API const std::string& GetDirectoryParsed(size_t index) const { return m_directoriesParsed.at(index); }
		REFLECT_API const std::vector<std::string>& GetAllDirectoriesParsed() const { return  m_directoriesParsed; }

	private:
		std::ifstream OpenFile(const std::string& filePath);
		void CloseFile(std::ifstream& file);

		bool CheckExtension(const std::string& filePath, std::vector<const char*> extensions);
		bool CheckIfAutoGeneratedFile(const std::string& filePath);

		FileParsedData LoadFile(std::ifstream& file);

		bool ParseFile(FileParsedData& fileData);
		void ParseUsingTags(FileParsedData& fileData);

		uint64_t FindNextContainer(FileParsedData& fileData) const;
		uint64_t FindNextReflectContainer(FileParsedData& fileData) const;

		bool ParseContainerReflectProperties(FileParsedData& fileData) const;
		bool ParseContainerHeader(FileParsedData& fileData) const;

		bool FileHasReflectData(FileParsedData& fileData, const std::string& keyword, const EReflectType type) const;
		bool ReflectContainerHeader(FileParsedData& fileData, const std::string& keyword, const EReflectType type);
		void ReflectContainer(FileParsedData& fileData) const;
		void GetAllCPPIncludes(FileParsedData& fileData) const;

		std::vector<std::string> FindAllNamespaces(FileParsedData fileData, size_t reflectStart) const;
		std::string FindPreviousNamespace(FileParsedData& fileData) const;

		std::vector<std::string> FindAllIfDefines(FileParsedData fileData, size_t reflectStart) const;

		size_t FindEndOfConatiner(const FileParsedData& fileData) const;
		std::vector<std::string> ReflectFlags(FileParsedData fileData) const;
		std::vector<PropertyMeta> ReflectPropertyMetas(FileParsedData fileData) const;

		void ResolveNamespaces();
		void LinkAllInheritances();
		void RemoveLookupOnlyContainers();

		char FindNextChar(FileParsedData& fileData, const std::vector<char>& ignoreChars, bool reverse = false) const;
		char FindNextChar(FileParsedData const& fileData, size_t& cursor, const std::vector<char>& ignoreChars, bool reverse = false) const;
		char FindNextChar(FileParsedData& fileData, char charToFind, bool reverse = false) const;

		std::string FindNextWord(FileParsedData& fileData, const std::vector<char>& endChars, bool reverse = false) const;
		std::string FindNextWord(const FileParsedData& fileData, size_t& cursor, const std::vector<char>& endChars, bool reverse = false) const;
		bool IsWordReflectKey(std::string_view view) const;

		void MoveToEndOfScope(FileParsedData& fileData, const char startScopeChar, const char endScopeChar) const;

		bool CheckForTypeAlias(std::string_view view) const;
		bool CheckForVisibility(std::string_view view) const;
		bool CheckForConstructor(FileParsedData& fileData, Parser::ReflectContainerData& container, std::string_view view) const;
		bool CheckForIgnoreWord(FileParsedData& fileData, std::string_view view) const;
		bool CheckForOperatorFunction(FileParsedData& fileData, std::string_view view) const;
		bool CheckForComments(FileParsedData& fileData, std::string& line) const;
		bool CheckForFriends(FileParsedData& fileData, std::string_view view) const;

		uint64_t CheckForTemplate(FileParsedData& fileData) const;
		std::vector<ReflectTemplateData> ParseTemplateData(FileParsedData& fileData) const;
		uint64_t FindEndOfTemplate(const FileParsedData& fileData, const uint64_t tempalteStartIdx) const;

		bool IsCursorWithinComment(FileParsedData& fileData, uint64_t& cursor, const bool moveCursorToEnd) const;

		const char* GetLastReflectContainerKey(const FileParsedData& fileData) const;

		void GetReflectNameAndReflectValueTypeAndReflectModifer(std::string& str, std::string& name, EReflectValueType& valueType, EReflectValueModifier& modifer) const;

		Parser::ReflectFunctionData GetFunction(FileParsedData& fileData, const std::vector<std::string>& flags) const;
		ReflectMemberData GetMember(FileParsedData& fileData, const std::vector<std::string>& flags) const;

		void SkipFunctionBody(FileParsedData& fileData) const;

		EReflectType CheckForReflectType(FileParsedData& data) const;

		bool CheckForEndOfFile(FileParsedData& fileData, size_t cursor) const;
		EReflectValueType CheckForRefOrPtr(std::string_view view) const;
		EReflectValueModifier CheckForMemberModifers(std::string_view view) const;

		std::string GetFunctionLine(const FileParsedData& fileData, size_t& endCursor) const;
		std::vector<Parser::ReflectTypeNameData> ReflectGetFunctionParameters(const FileParsedData& fileData, std::string_view view) const;

		void CheckStringViewBounds(const FileParsedData& fileData, size_t cursor, std::string_view view) const;
		size_t CountNumberOfSinceTop(const FileParsedData& fileData, size_t cursorStart, const char& character) const;

		std::string PrettyString(std::string str) const;
		ReflectContainerData* FindReflectContainerData(std::string_view containerName);
		const ReflectContainerData* FindReflectContainerData(std::string_view containerName) const;
		std::string FindFullAliasName(const std::string& aliasName) const;

	private:
		std::vector<FileParsedData> m_filesParsed;
		std::vector<std::string> m_ignoreStrings;
		std::vector<std::string> m_directoriesParsed;
		const ReflectAddtionalOptions* m_options;

		uint64_t m_toalNumberOfContainersParsed = 0;
		uint64_t m_toalNumberOfContainersToGenerateCode = 0;

		std::unordered_map<std::string, std::vector<std::string>> TypeAliasMap;
	};
}
