cmake_minimum_required(VERSION 3.10)

project(CoreEngine LANGUAGES CXX)

if(UNIX AND NOT APPLE)
    set(PLATFORM_LINUX 1)
    message(STATUS "Platform: Linux")
elseif(APPLE)
    set(PLATFORM_MACOS 1)
    message(STATUS "Platform: macOS")
elseif(WIN32)
    set(PLATFORM_WINDOWS 1)
    message(STATUS "Platform: Windows")
else()
    message(WARNING "Unknown platform")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(MSVC)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Release Debug MinSizeRel RelWithDebInfo)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
    set(CMAKE_BUILD_TYPE Release)
    cmake_policy(SET CMP0091 NEW)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded" CACHE STRING "MSVC runtime" FORCE)
endif()

# Core headers
file(GLOB_RECURSE CORE_HEADERS "Core/Source/*.h" "Core/Dependencies/imgui/*.h")

# Core sources
file(GLOB_RECURSE CORE_SOURCES "Core/Source/*.cpp" "Core/Dependencies/imgui/*.cpp")

# Core (static lib)
add_library(Core STATIC ${CORE_HEADERS} ${CORE_SOURCES})

target_compile_definitions(Core PRIVATE FREEIMAGE_LIB)

if(PLATFORM_LINUX)
    set_target_properties(Core PROPERTIES OUTPUT_NAME "libCore.a")
elseif(PLATFORM_WINDOWS)
    set_target_properties(Core PROPERTIES OUTPUT_NAME "Core")
endif()

find_package(OpenGL REQUIRED COMPONENTS OpenGL)

if(PLATFORM_LINUX)
    target_link_libraries(Core PRIVATE
        OpenGL::OpenGL
    )
    target_link_libraries(Core PRIVATE uuid)
elseif(PLATFORM_WINDOWS)
    target_link_libraries(Core PRIVATE OpenGL::GL)
endif()

target_include_directories(Core PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/FreeImage/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/glm/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/bc7enc/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/assimp/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/glew/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/icu/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/sdl/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/imgui
)

add_library(Assimp STATIC IMPORTED)
add_library(ICUDT STATIC IMPORTED)
add_library(ICUUC STATIC IMPORTED)
add_library(SDL2 STATIC IMPORTED)
add_library(GLEW STATIC IMPORTED)
add_library(FreeImage STATIC IMPORTED)
add_library(bc7enc STATIC IMPORTED)
add_library(glm STATIC IMPORTED)

set_target_properties(Assimp PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/assimp/include"
)
set_target_properties(GLEW PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/glew/include"
)
set_target_properties(ICUDT PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/icu/include"
)
set_target_properties(ICUUC PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/icu/include"
)
set_target_properties(SDL2 PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/sdl/include"
)
set_target_properties(FreeImage PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/FreeImage/include"
)
set_target_properties(bc7enc PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/bc7enc/include"
)
set_target_properties(glm PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/glm/include"
)

if(PLATFORM_LINUX)
    set_target_properties(Assimp PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/assimp/bin/linux64/libassimp.so.6.0.2"
    )
    set_target_properties(GLEW PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/glew/bin/linux64/libGLEW.so.2.2.0"
    )
    set_target_properties(ICUDT PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/icu/lib/linux64/libicudata.a"
    )
    set_target_properties(ICUUC PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/icu/lib/linux64/libicuuc.a"
    )
    set_target_properties(SDL2 PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/sdl/bin/linux64/libSDL2-2.0.so.0.2600.3"
    )
    set_target_properties(FreeImage PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/FreeImage/lib/linux64/libFreeImage.a"
    )
    set_target_properties(bc7enc PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/bc7enc/lib/linux64/libbc7enc.a"
    )
    set_target_properties(glm PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/glm/lib/linux64/libglm_static.a"
    )
elseif(PLATFORM_WINDOWS)
    set_target_properties(Assimp PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/assimp/lib/win64/assimp-vc143-mt.lib"
    )
    set_target_properties(GLEW PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/glew/lib/win64/glew32s.lib"
    )
    set_target_properties(ICUDT PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/icu/lib/win64/icudt.lib"
    )
    set_target_properties(ICUUC PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/icu/lib/win64/icuuc.lib"
    )
    set_target_properties(SDL2 PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/sdl/lib/win64/SDL2-static.lib"
    )
    set_target_properties(FreeImage PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/FreeImage/lib/win64/FreeImage.lib"
    )
    set_target_properties(bc7enc PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/bc7enc/lib/win64/bc7enc.lib"
    )
    set_target_properties(glm PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/glm/lib/win64/glm_static.lib"
    )
endif()

target_link_libraries(Core
    PRIVATE Assimp
    PRIVATE ICUDT
    PRIVATE ICUUC
    PRIVATE SDL2
    PRIVATE GLEW
    PRIVATE FreeImage
    PRIVATE bc7enc
    PRIVATE glm
)
# Editor headers
file(GLOB_RECURSE EDITOR_HEADERS "Editor/Source/*.h")

# Editor sources
file(GLOB_RECURSE EDITOR_SOURCES "Editor/Source/*.cpp")

# Editor (executable, depends on Core)
add_executable(Editor ${EDITOR_HEADERS} ${EDITOR_SOURCES})

if(PLATFORM_LINUX)
    set_target_properties(Editor PROPERTIES OUTPUT_NAME "Editor.app")
elseif(PLATFORM_WINDOWS)
    set_target_properties(Editor PROPERTIES OUTPUT_NAME "Editor")
    target_link_options(Editor PRIVATE "/NODEFAULTLIB:msvcrt.lib") 
    target_link_libraries(Core PRIVATE winmm imm32 version setupapi)
endif()

if(PLATFORM_LINUX)
    set_target_properties(Editor PROPERTIES
        BUILD_RPATH "$ORIGIN"
        INSTALL_RPATH "$ORIGIN"
    )
endif()

target_include_directories(Editor PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Source
    ${CMAKE_CURRENT_SOURCE_DIR}/Editor/Dependencies/Carve/include
)

add_library(Carve STATIC IMPORTED)
set_target_properties(Carve PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Editor/Dependencies/Carve/include"
)

if(PLATFORM_LINUX)
set_target_properties(Carve PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Editor/Dependencies/Carve/lib/linux64/libcarve.a"
)
elseif(PLATFORM_WINDOWS)
set_target_properties(Carve PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Editor/Dependencies/Carve/lib/win64/carve.lib"
)
endif()

# Link Editor and Core
target_link_libraries(Editor
    PRIVATE Core
    PRIVATE Carve
)

# Group projects
set_property(TARGET Core PROPERTY FOLDER "Core")
set_property(TARGET Editor PROPERTY FOLDER "Editor")

# Output directory
if(PLATFORM_LINUX)
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/linux64)
    set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/linux64)
elseif(PLATFORM_WINDOWS)
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/win64)
    set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/win64)
endif()

if(PLATFORM_LINUX)
    add_custom_command(
        TARGET Editor
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Resources" "${CMAKE_BINARY_DIR}/linux64/${CMAKE_BUILD_TYPE}/Editor"
    )
elseif(PLATFORM_WINDOWS)
    add_custom_command(
        TARGET Editor
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Resources" "${CMAKE_BINARY_DIR}/win64/${CMAKE_BUILD_TYPE}/Editor/"
    )
endif()