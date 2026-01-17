workspace "NanSu-Engine"
    architecture "x64"
    startproject "Editor"

    configurations { "Debug", "Release", "Distribution" }

    -- 결과물 출력 경로 (예: Binaries/Debug-windows-x64/Editor/)
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    -- 공통 설정
    filter "system:windows"
        systemversion "latest"
        defines { "NS_PLATFORM_WINDOWS" }

    filter "configurations:Debug"
        defines { "NS_DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NS_RELEASE" }
        runtime "Release"
        optimize "On"

    filter "configurations:Distribution"
        defines { "NS_DISTRIBUTION" }
        runtime "Release"
        optimize "On"

--------------------------------------------------------------------------------
-- 1. Engine (라이브러리)
--------------------------------------------------------------------------------
project "Engine"
    location "Source/Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "On"

    targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("Build/" .. outputdir .. "/%{prj.name}")

    -- Precompiled Header
    pchheader "EnginePCH.h"
    pchsource "Source/Engine/EnginePCH.cpp"

    files {
        "Source/Engine/**.h",
        "Source/Engine/**.cpp",
        -- ImGui core
        "ThirdParty/imgui/imgui.cpp",
        "ThirdParty/imgui/imgui_demo.cpp",
        "ThirdParty/imgui/imgui_draw.cpp",
        "ThirdParty/imgui/imgui_tables.cpp",
        "ThirdParty/imgui/imgui_widgets.cpp",
        -- ImGui backends
        "ThirdParty/imgui/backends/imgui_impl_win32.cpp",
        "ThirdParty/imgui/backends/imgui_impl_dx11.cpp"
    }

    includedirs {
        "Source",
        "Source/Engine",
        "ThirdParty/spdlog/include",
        "ThirdParty/imgui",
        "ThirdParty/imgui/backends",
        "ThirdParty/glm"
    }

    -- ImGui 파일은 PCH 제외
    filter "files:ThirdParty/imgui/**.cpp"
        flags { "NoPCH" }
    filter {}

    buildoptions { "/utf-8" }

    -- Note: DirectX 11 libraries are linked via #pragma comment(lib, ...) in DX11Context.cpp

--------------------------------------------------------------------------------
-- 2. Editor (실행 파일)
--------------------------------------------------------------------------------
project "Editor"
    location "Source/Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "On"

    targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("Build/" .. outputdir .. "/%{prj.name}")

    files {
        "Source/Editor/**.h",
        "Source/Editor/**.cpp"
    }

    includedirs {
        "Source",
        "Source/Engine",
        "ThirdParty/spdlog/include",
        "ThirdParty/imgui",
        "ThirdParty/glm"
    }

    links { "Engine" }

    buildoptions { "/utf-8" }

--------------------------------------------------------------------------------
-- 3. Game (샌드박스)
--------------------------------------------------------------------------------
project "Game"
    location "Source/Game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "On"

    targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("Build/" .. outputdir .. "/%{prj.name}")

    files {
        "Source/Game/**.h",
        "Source/Game/**.cpp"
    }

    includedirs {
        "Source",
        "Source/Engine",
        "ThirdParty/spdlog/include",
        "ThirdParty/imgui",
        "ThirdParty/glm"
    }

    links { "Engine" }

    buildoptions { "/utf-8" }