#include "Reflect.h"

STRUCT() Person
{
	int x;
	int y;
};

int main(void)
{
	Reflect::FileParser parser;
	parser.ParseDirectory("F:/Users/Documents/SourceControl/Github/C++ Porjects/Reflect/ReflectTest/src");
	return 0;
}