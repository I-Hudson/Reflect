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


group "Dependencies"
group ""

include "Reflect"
--nclude "ReflectDLL"
include "ReflectEXE"
include "ReflectTest"