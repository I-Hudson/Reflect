#include "Reflect.h"

int main(int argc, char* argv[])
{
	PROFILE_BEGIN_SESSION();
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

	PROFILE_END_SESSION();
	PROFILE_SAVE_SESSION("ReflectEXE_Profile.json");
	return 0;
}