#include "Reflect.h"

int main(int argc, char* argv[])
{
	PROFILE_BEGIN_SESSION();
	Reflect::Profile::InstrumentationTimer timer("Reflect Timer");
	{
		PROFILE_SCOPE("MAIN");

		Reflect::FileParser parser;
		Reflect::CodeGenerate codeGenerate;
		for (size_t i = 0; i < argc; ++i)
		{
			parser.ParseDirectory(argv[i]);
			for (auto& file : parser.GetAllFileParsedData())
			{
				codeGenerate.Reflect(file);
			}
		}
	}
	timer.Stop();
	std::cout << "Reflect micro: " << timer.GetElapsedTimeMicro() << std::endl;
	std::cout << "Reflect mill: " << timer.GetElapsedTimeMill() << std::endl;
	std::cout << "Reflect sec: " << timer.GetElapsedTimeSec() << std::endl;
	PROFILE_END_SESSION();
	PROFILE_SAVE_SESSION("ReflectEXE_Profile.json");
	return 0;
}