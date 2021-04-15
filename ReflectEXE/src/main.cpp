#include "Reflect.h"
#include <filesystem>
#include <string_view>

int main(int argc, char* argv[])
{
	REFLECT_PROFILE_BEGIN_SESSION();
	Reflect::Profile::InstrumentationTimer timer("Reflect Timer");
	{
		REFLECT_PROFILE_SCOPE("MAIN");

		Reflect::FileParser parser;
		Reflect::CodeGenerate codeGenerate;
		Reflect::CodeGenerateAddtionalOptions options = { };

		std::vector<std::string> directories;

		for (size_t i = 0; i < argc; ++i)
		{
			if (std::filesystem::is_directory(argv[i]))
			{
				directories.push_back(argv[i]);
			}
			else
			{
				std::string arg = argv[i];
				if (arg.find("pchInclude=") != std::string::npos)
				{
					options.IncludePCHString = arg.substr(strlen("pchInclude="));
				}
			}
		}

		for (auto& dir : directories)
		{
			parser.ParseDirectory(dir);
			for (auto& file : parser.GetAllFileParsedData())
			{
				codeGenerate.Reflect(file, options);
			}
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