#include "Reflect/Reflect.h"
#include <filesystem>
#include <string_view>
#include <string.h>
#pragma optimize("", off)
int main(int argc, char* argv[])
{
	REFLECT_PROFILE_BEGIN_SESSION();
	Reflect::Profile::InstrumentationTimer timer("Reflect Timer");
	{
		REFLECT_PROFILE_SCOPE("MAIN");

		Reflect::Parser::FileParser parser;
		Reflect::CodeGeneration::CodeGenerate codeGenerate;
		Reflect::ReflectAddtionalOptions options = { };

		std::vector<std::string> directories;
		for (size_t i = 0; i < argc; ++i)
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
			{ }

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
		}

		if (directories.size() > 0)
		{
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
				parser.SetIgnoreStrings(Reflect::Util::SplitString(data.data(), '\n'));
			}

			for (auto& dir : directories)
			{
				parser.ParseDirectory(dir, &options);
			}
			codeGenerate.Reflect(parser, &options);
		}
	}
	timer.Stop();
	std::cout << "Reflect micro: " << timer.GetElapsedTimeMicro() << std::endl;
	std::cout << "Reflect mill: " << timer.GetElapsedTimeMill() << std::endl;
	std::cout << "Reflect sec: " << timer.GetElapsedTimeSec() << std::endl;
	REFLECT_PROFILE_END_SESSION();
	REFLECT_PROFILE_SAVE_SESSION("ReflectEXE_Profile.json");
	return 0;
}