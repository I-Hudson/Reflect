workspace "Reflect"
    architecture "x64"
    startproject "ReflectTest"
    toolset "msc-v143"

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
        "REFLECT_TYPE_INFO_ENABLED",
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["refect"] = "$(SolutionDir)Reflect/"


group "Dependencies"
group ""

include "Reflect"
include "ReflectEXE"
include "ReflectTest"