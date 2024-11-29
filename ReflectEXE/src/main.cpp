#include "Reflect/Reflect.h"
#include "Reflect/Core/Log.h"
#include "Reflect/Instrumentor.h"
#include "Reflect/FileParser/FileParser.h"
#include "Reflect/CodeGenerate/CodeGenerate.h"

#include <filesystem>
#include <string_view>
#include <string.h>

#ifdef _WIN32
#include "../resource.h"
#include <Windows.h>
#endif 

const char* CommandPrefixInfo[3] = { "Commands are defined with the prefix '-'", "Example '-Help'", "" };
const char* CommandNoValueInfo[3] = { "Commands without the '=<Values>' subfix must be in the format of '-CommandId", "'-DontCloseOnFinish'", "" };
const char* CommandValueInfo[3] = { "Commands with the '=<Values>' subfix must be in the format of '-CommandId'='Value1' 'Value2' ...", "-Directories='LocalHeaders' '../Vendor/Headers'", "" };

const char* CommandHelp[3] =				{ "-Help", "-Help", "Print all commands." };
const char* CommandVersion[3] =				{ "-Version", "-Version", "Display the current version." };
const char* CommandDontCloseOnFinish[3] =	{ "-DontCloseOnFinish", "-DontCloseOnFinish", "Don't close the app after Reflect has finished parsing and code generation." };
const char* CommandDirectories[3] =			{ "-Directories", "Directories=<Values>", "Define multiple directories to parse." };
const char* CommandIgnoreWords[3] =			{ "-IgnoreWords", "-IgnoreWords=<Values>", "Define words to ignore when parsing (Example: defines used for __declspec(dllexport) and __declspec(dllimport))." };
const char* CommandEnableProfileStats[3] =	{ "-EnableProfileStats", "-EnableProfileStats", "Enable profiling stats." };
const char* CommandExit[3] =				{ "-Exit", "-Exit", "Exit the application." };

#define COMMAND_GET_ID(Command) Command[0]
#define COMMAND_GET_EXAMPLE(Command) Command[1]
#define COMMAND_GET_DESCRIPTION(Command) Command[2]

#define COMMAND_LOG_VALUES(CommandArray) \
{ CommandArray[0], { CommandArray[1], CommandArray[2]} }

const std::unordered_map<std::string, std::vector<const char*>> Commands =
{
	COMMAND_LOG_VALUES(CommandPrefixInfo),
	COMMAND_LOG_VALUES(CommandNoValueInfo),
	COMMAND_LOG_VALUES(CommandValueInfo),

	COMMAND_LOG_VALUES(CommandHelp),
	COMMAND_LOG_VALUES(CommandDontCloseOnFinish),
	COMMAND_LOG_VALUES(CommandDirectories),
	COMMAND_LOG_VALUES(CommandIgnoreWords),
	COMMAND_LOG_VALUES(CommandExit),
};
#undef COMMAND_LOG_VALUES

std::unordered_map<std::string, std::vector<std::string>> ParseInputToCommands(const std::string& input);
void RunReflect(const std::unordered_map<std::string, std::vector<std::string>>& commands);

int main(int argc, char* argv[])
{
#ifdef _WIN32
	HWND hWnd = GetConsoleWindow();
	HINSTANCE hInstance = GetModuleHandle(NULL);
	if (hWnd && hInstance)
	{
		HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
		SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	}
#endif

	std::cout << R"(
 ____       __ _           _   
|  _ \ ___ / _| | ___  ___| |_ 
| |_) / _ \ |_| |/ _ \/ __| __|
|  _ <  __/  _| |  __/ (__| |_ 
|_| \_\___|_| |_|\___|\___|\__|)" << "\n\n";

	std::cout << "Version: " << REFLECT_MAJOR << "." << REFLECT_MINOR << "." << REFLECT_PATCH << "\n";
	std::cout << "Use '-Help' for a list of all commands. Commands are case sensitive." << "\n\n\n";


	std::string cmdArguments;
	for (size_t i = 1; i < argc; i++)
	{
		cmdArguments += argv[i];
		cmdArguments += " ";
	}
	std::unordered_map<std::string, std::vector<std::string>> commands = ParseInputToCommands(cmdArguments);

	bool shouldExit = !(commands.find(COMMAND_GET_ID(CommandDontCloseOnFinish)) != commands.end());
	RunReflect(commands);

	while (!shouldExit)
	{
		std::string input;
		std::cin >> input;

		commands = ParseInputToCommands(input);
		RunReflect(commands);

		if (commands.find(COMMAND_GET_ID(CommandHelp)) != commands.end())
		{
			Log_Info("\n");
			Log_Info("/**************\n");
			Log_Info("All commands, example usage and description of commands are below.\n");
			Log_Info("[Id] [Usage] [Description].\n");
			for (const auto& kvp : Commands)
			{
				Log_Info("%s, %s, %s\n\n", kvp.first, kvp.second[0], kvp.second[1]);
			}
			Log_Info("/**************\n");
		}
		if (commands.find(COMMAND_GET_ID(CommandVersion)) != commands.end())
		{
			Log_Info("Version: %d.%d.%d\n", REFLECT_MAJOR, REFLECT_MINOR, REFLECT_PATCH);
		}
		if (commands.find(COMMAND_GET_ID(CommandExit)) != commands.end())
		{
			shouldExit = true;
		}
	}
}

std::unordered_map<std::string, std::vector<std::string>> ParseInputToCommands(const std::string& input)
{
	std::vector<std::string> inputSplit = Reflect::Util::SplitString(input, ' ');

	std::unordered_map<std::string, std::vector<std::string>> commands;

	// Parse the initial arguments passed into the application. 
	std::string commandId;
	for (size_t i = 0; i < inputSplit.size(); i++)
	{
		std::string str = inputSplit[i];
		if (str.empty())
		{
			continue;
		}

		const bool strIsCommandId = !str.empty() && str[0] == '-';
		if (strIsCommandId)
		{
			commandId = str.substr(0, str.find('='));
			if (Commands.find(commandId.c_str()) == Commands.end())
			{
				Log_Error("[Reflect] Unknown command '%s'.\n", commandId.c_str());
				continue;
			}

			commands[commandId];

			const uint64_t valuePos = str.find("=");
			if (valuePos != std::string::npos)
			{
				commands[commandId].push_back(str.substr(valuePos + 1));
			}
		}
		else if (!commandId.empty())
		{
			commands[commandId].push_back(str);
		}
		else
		{
			Log_Error("[Reflect] No command for value '%s'.\n", str.c_str());
		}
	}
	return commands;
}

void RunReflect(const std::unordered_map<std::string, std::vector<std::string>>& commands)
{
	std::vector<std::string> directories;
	if (const auto iter = commands.find(COMMAND_GET_ID(CommandDirectories));
		iter != commands.end())
	{
		std::copy(iter->second.begin(), iter->second.end(), std::back_inserter(directories));
	}
	/*for (size_t i = 0; i < argc; ++i)
	{
		bool foundPath = false;
		try
		{
			std::filesystem::path absPath = std::filesystem::absolute(argv[i]);
			if (std::filesystem::is_directory(absPath))
			{
				directories.push_back(absPath.string());
				foundPath = true;
			}
		}
		catch (std::error_code)
		{
		}

		if (!foundPath)
		{
			std::string arg = argv[i];
			std::string argKey = arg.substr(0, arg.find('='));
			std::unordered_map<const std::string, std::string>::iterator itr = options.options.find(argKey);
			if (itr != options.options.end())
			{
				itr->second = arg.substr(arg.find('=') + 1);
			}
		}
	}*/

	std::vector<std::string> ignoreStrings;
	if (const auto iter = commands.find(COMMAND_GET_ID(CommandIgnoreWords));
		iter != commands.end())
	{
		std::copy(iter->second.begin(), iter->second.end(), std::back_inserter(ignoreStrings));
	}

	if (directories.size() > 0)
	{
		Reflect::Parser::FileParser parser;
		Reflect::CodeGeneration::CodeGenerate codeGenerate;
		Reflect::ReflectAddtionalOptions options = { };

		std::ifstream iFile(Reflect::Keys::ReflectIgnoreStringsFileName);
		if (iFile.is_open())
		{
			iFile.seekg(0, iFile.end);
			size_t size = iFile.tellg();
			iFile.seekg(0, iFile.beg);

			std::string data;
			data.resize(size);
			iFile.read(data.data(), size);
			iFile.close();

			const std::vector<std::string> ignoreStringsFile = Reflect::Util::SplitString(data.data(), '\n');
			std::move(ignoreStringsFile.begin(), ignoreStringsFile.end(), std::back_inserter(ignoreStrings));
		}
		parser.SetIgnoreStrings(ignoreStrings);

		const bool profilingEnabled = commands.find(COMMAND_GET_ID(CommandEnableProfileStats)) != commands.end();

		Reflect::Profile::InstrumentationTimer timer("Reflect Timer");
		REFLECT_PROFILE_BEGIN_SESSION();
		REFLECT_PROFILE_SCOPE("MAIN");

		for (auto& dir : directories)
		{
			parser.ParseDirectory(dir, &options);
		}
		codeGenerate.Reflect(parser, &options);

		timer.Stop();
		if (profilingEnabled)
		{
			std::cout << "Reflect micro: " << timer.GetElapsedTimeMicro() << "\n";
			std::cout << "Reflect mill: " << timer.GetElapsedTimeMill() << "\n";
			std::cout << "Reflect sec: " << timer.GetElapsedTimeSec() << "\n";
		}
		REFLECT_PROFILE_END_SESSION();
		if (profilingEnabled)
		{
			REFLECT_PROFILE_SAVE_SESSION("Reflect_Profile.json");
		}
		Reflect::Log::Message(Reflect::Log::ConsoleColour::Light_Green, Reflect::Log::ConsoleColour::Black, "Reflect has finished.\n");
	}
}