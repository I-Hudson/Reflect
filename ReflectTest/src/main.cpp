#include "Reflect.h"
#include "TestStrcuts.h"
#include <iostream>

int main(void)
{
	Reflect::FileParser parser;
	parser.ParseDirectory("F:/Users/Documents/SourceControl/Github/C++ Porjects/Reflect/ReflectTest/src");

	const auto& fileData = parser.GetFileParsedData(0);

	return 0;
}