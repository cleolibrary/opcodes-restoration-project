workspace "opcodes-restoration-project"
    configurations { "Release", "Debug" }
    architecture "x86"
    location "build"
    objdir "build/obj"
    buildlog "build/log/%{prj.name}.log"

    language "C++"
    characterset "MBCS"
    flags { "StaticRuntime", "NoBufferSecurityCheck" }
    floatingpoint "Fast"
    vectorextensions "IA32"

    includedirs {
        "external/gtalc-dinput8/vcclasses/include",
        "external/gtalc-dinput8/vcversion/include",
        "external/gtalc-dinput8/dinput8/include"
    }
    libdirs { "bin" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "III.Opcodes"
    defines { "_III" }
    exceptionhandling "Off"
    files { "src/dllmain.cpp" }
    includedirs { "external/III.VC.CLEO/source/CLEO_SDK" }
    kind "SharedLib"
    links { "III.CLEO" }
    targetdir "bin/CLEO/CLEO_PLUGINS"
    targetextension ".cleo"
    warnings "Extra"
project "VC.Opcodes"
    defines { "_VC" }
    exceptionhandling "Off"
    files { "src/*.h", "src/*.cpp" }
    includedirs { "external/III.VC.CLEO/source/CLEO_SDK" }
    kind "SharedLib"
    links { "vcclasses", "vcversion", "VC.CLEO" }
    targetdir "bin/CLEO/CLEO_PLUGINS"
    targetextension ".cleo"
    warnings "Extra"

project "III.CLEO"
    defines { "CLEO_III", "_CRT_SECURE_NO_WARNINGS" }
    files {
        "external/III.VC.CLEO/source/III.VC.CLEO/*.h",
        "external/III.VC.CLEO/source/III.VC.CLEO/*.cpp"
    }
    includedirs { "external/III.VC.CLEO/external/injector/include" }
    kind "SharedLib"
    targetdir "bin"
    targetextension ".asi"
project "VC.CLEO"
    defines { "CLEO_VC", "_CRT_SECURE_NO_WARNINGS" }
    files {
        "external/III.VC.CLEO/source/III.VC.CLEO/*.h",
        "external/III.VC.CLEO/source/III.VC.CLEO/*.cpp"
    }
    includedirs { "external/III.VC.CLEO/external/injector/include" }
    kind "SharedLib"
    targetdir "bin"
    targetextension ".asi"

project "vcclasses"
    exceptionhandling "Off"
    files {
        "external/gtalc-dinput8/vcclasses/include/*.h",
        "external/gtalc-dinput8/vcclasses/src/*.cpp"
    }
    kind "StaticLib"
    links { "vcversion" }
    targetdir "bin"
project "vcversion"
    exceptionhandling "Off"
    files {
        "external/gtalc-dinput8/vcversion/include/*.h",
        "external/gtalc-dinput8/vcversion/src/*.cpp"
    }
    kind "StaticLib"
    targetdir "bin"
