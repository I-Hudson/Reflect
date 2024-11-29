
# Reflect
![Logo](Logo/ReflectLogo.png)

TODO: update examples to match API changes.

This goal of this project was to create something similar to Unreal Engine's header tool and to see how auto generated code could be done. The main features of the project are a file parser and code generation. 

## How does it work?
First Reflect will parse files within a directory and collect relevant information about any container (struct/class) it needs. The minimal amount of information collect are the container name, namespaces the container is defined in and any inheritances a container has.

After parsing has been completed. Reflect will then generate the require code for all containers which require it.

To define a container to be reflected the container must have the <span style="color:#bfb7ff">REFLECT_\[STRUCT/CLASS\]()</span> macro defined for it. If a container has this macro defined for it then the body of the container must also have <span style="color:#bfb7ff">REFLECT_GENERATED_BODY()</span>. This is to allow for the reflected code to be added. Lastly the appropriate header (which is generated to a folder called 'Generated' in the same directory as the source header file) must be included.
```cpp
REFLECT_STRUCT()
struct SomeStruct
{
	REFLECT_GENERATED_BODY();
	int x;
	int y;
	...
};

REFLECT_CLASS()
class SomeClass
{
	REFLECT_GENERATED_BODY()
	public:
	std::string Name
	...
};
```



## Compilers 
C++17 required
- MSVC 
- LLVM/clang-cl
 
## Building
To build the project run the GENERATE_PROJECT.bat file in the root directory and premake should take care of the rest.

## Command line args
- pchInclude (optional): This will add an include to all generated .cpp files to the supplied header.
Example: pchInclude=pch.h 
- reflect_full_ext (optional): This will attempted to reflect any class/struct which has the appropriate REFLECT_ tag (REFLECT_CLASS()/REFLECT_STRUCT()).
This must be either ture or false.
This is an extension feature and is not guaranteed to work as Reflect doesn't have all the information about the code.
Example: reflect_full_ext=true

## Example 
```cpp
REFLECT_CLASS()
class Person
{
	REFLECT_GENERATED_BODY()
public:
	Person() { }
	
	REFLECT_PROPERTY()
	int y;
	REFLECT_PROPERTY()
	int x;

	REFLECT_PROPERTY()
	int TestFuncInt(int const* testInt, const int& testIntPtr) { std::cout << "TestFunc is being called"; return *testInt + testIntPtr; }
	REFLECT_PROPERTY()
	const char* TestFuncChar() { std::cout << "TestFuncChar"; return ""; }
	REFLECT_PROPERTY()
	void TestFuncVoid() { std::cout << "TestFunc is being called"; }

	REFLECT_PROPERTY()
	std::string TestString;
};
```

## How to use
There are two ways to use this tool. The first and easiest is through the .exe console app. Run the console and add the folders in which you want the tools to search for header files.
Example: C:\Reflect.exe C:\HeaderFiles
The tool will iterate over all the header files in C:\HeaderFiles and look for the Reflect macros. If one is found then a new .h and .cpp will be generated into a new folder called "Generated" in the same directory as the original header file.

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

## Example
Am example of how to use this tool is in ReflectTest.
```cpp
void FuncNoReturn()
{
	// Get a function with no return value.
	Player player;
	auto playerGetId = player.GetFunction("PrintHelloWorld");
	playerGetId.Invoke();
}

void FuncReturnValue()
{
	// Get a function with a return value std::string.
	// The return value with be set to playerId.
	Player player;
	ReflectFunction playerGetId = player.GetFunction("GetId");
	std::string playerId;
	playerGetId.Invoke(&playerId);
}

void FuncWithParameters()
{
	// Get a function with no return value but which has a single
	// parameter.
	Player player;
	ReflectFunction parameterFunc = player.GetFunction("GetOnlineFriendsCountt");
	
	// Setup the parameter to send to the function. This is order
	// sensitive.
	FunctionPtrArgs args;
	int intParameter = 8;
	args.AddArg(&intParameter);

	int returnCount;
	std::cout << ReflectReturnCodeToString(parameterFunc.Invoke(&returnCount, args));
}

int main(void)
{
	FuncNoReturn();
	FuncReturnValue();
	FuncWithParameters();

	return 0;
}
```
