#include "Reflect.h"

int main(int argc, char* arg[])
{
	Reflect::FileParser parser;
	Reflect::CodeGenerate codeGenerate;
	for (size_t i = 0; i < argc; ++i)
	{
		parser.ParseDirectory(arg[i]);
		for (auto& file : parser.GetAllFileParsedData())
		{
			codeGenerate.Reflect(file);
		}
	}

	return 0;
}