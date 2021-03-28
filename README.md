# Reflect

This goal of this project was to create something similar to Unreal Engine's header tool and to see how auto generated code could be done.

## Compilers 
- MSVC C++17 required

## Building
To build the project run the GENERATE_PROJECT.bat file in the root directory and premake should take care of the rest.

## How to use
There are two ways to use this tool. The first and easiest is through the .exe console app. Run the console and add the folders in which you want the tools to search for header files.
Example:
C:\Reflect.exe C:\HeaderFiles
The tool will interate over all the header files in C:\HeaderFiles and look for the Reflect macros. If one is found then a new .h and .cpp will be generated into a new folder called "Generated" in the same directroy as the original header file.

The second way to use this tool is through the .lib and header files. Include the header files from the release .zip and link to the .lib. From here you will need to call the code generated functions from somewhere in your project.
```cpp
int main() 
{
   Reflect::FileParser parser;
   Reflect::CodeGenerate codeGenerate;
   
   parser.ParseDirectory("C:\HeaderFiles");
   for (auto& file : parser.GetAllFileParsedData())
   {
      codeGenerate.Reflect(file);
   }
}
```
