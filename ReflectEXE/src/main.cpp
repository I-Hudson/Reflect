#include "Reflect.h"

struct Test
{
	const char* c;
	int i;
};

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

#if DEBUG
	parser.ParseDirectory("F:/Users/Documents/SourceControl/Github/C++ Porjects/Reflect/ReflectTest/src");
	for (auto& file : parser.GetAllFileParsedData())
	{
		codeGenerate.Reflect(file);
	}
#endif

	return 0;
}