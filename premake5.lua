workspace "Reflect"
    architecture "x64"
    startproject "ReflectTest"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }
    
    flags
	{
		"MultiProcessorCompile"
    }
    
    defines
    {
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["refect"] = "$(SolutionDir)Reflect/"
IncludeDir["dirent"] = "$(SolutionDir)Reflect/vendor/dirent/include"


group "Dependencies"
group ""

include "Reflect"
include "ReflectTest"