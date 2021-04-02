#include "Reflect.h"

int main(int argc, char* argv[])
{
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
	return 0;
}