# Reflect

This goal of this project was to create something similar to Unreal Engine's header tool and to see how auto generated code could be done.

## Compilers 
- MSVC

## Building
To build the project run the GENERATE_PROJECT.bat file in the root directory and premake should take care of the rest.

## How to use
There are two ways to just this tool. The first is through the .lib and header files. Include the header files from the release .zip and link to the .lib. From here you will need to call the code generated functions from somewhere in your project.
```cpp
int main() 
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
}
```
