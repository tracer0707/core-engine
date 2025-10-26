cmake_minimum_required(VERSION 3.10)

project(CoreEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Core headers
file(GLOB_RECURSE CORE_HEADERS "Core/Source/*.h" "Core/Dependencies/imgui/*.h")

# Core sources
file(GLOB_RECURSE CORE_SOURCES "Core/Source/*.cpp" "Core/Dependencies/imgui/*.cpp")

# Core (static lib)
add_library(Core STATIC ${CORE_HEADERS} ${CORE_SOURCES})

find_package(OpenGL REQUIRED COMPONENTS OpenGL)

target_link_libraries(Core PRIVATE
    OpenGL::OpenGL
)

target_link_libraries(Core PRIVATE uuid)

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

add_library(Assimp SHARED IMPORTED)
set_target_properties(Assimp PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/assimp/bin/linux64/libassimp.so.6.0.2"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/assimp/include"
)

add_library(GLEW SHARED IMPORTED)
set_target_properties(GLEW PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/glew/bin/linux64/libGLEW.so.2.2.0"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/glew/include"
)

add_library(ICUDT STATIC IMPORTED)
set_target_properties(ICUDT PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/icu/lib/linux64/libicudata.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/icu/include"
)

add_library(ICUI18N STATIC IMPORTED)
set_target_properties(ICUI18N PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/icu/lib/linux64/libicui18n.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/icu/include"
)

add_library(ICUIO STATIC IMPORTED)
set_target_properties(ICUIO PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/icu/lib/linux64/libicuio.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/icu/include"
)

add_library(ICUTU STATIC IMPORTED)
set_target_properties(ICUTU PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/icu/lib/linux64/libicutu.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/icu/include"
)

add_library(ICUUC STATIC IMPORTED)
set_target_properties(ICUUC PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/icu/lib/linux64/libicuuc.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/icu/include"
)

add_library(SDL2 SHARED IMPORTED)
set_target_properties(SDL2 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/sdl/bin/linux64/libSDL2-2.0.so.0.2600.3"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/sdl/include"
)

add_library(FreeImage STATIC IMPORTED)
set_target_properties(FreeImage PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/FreeImage/lib/linux64/libFreeImage.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/FreeImage/include"
)

add_library(bc7enc STATIC IMPORTED)
set_target_properties(bc7enc PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/bc7enc/lib/linux64/libbc7enc.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/bc7enc/include"
)

add_library(glm STATIC IMPORTED)
set_target_properties(glm PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/glm/lib/linux64/libglm_static.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/glm/include"
)

target_link_libraries(Core PRIVATE Assimp)
target_link_libraries(Core PRIVATE GLEW)
target_link_libraries(Core PRIVATE ICUDT)
target_link_libraries(Core PRIVATE ICUI18N)
target_link_libraries(Core PRIVATE ICUIO)
target_link_libraries(Core PRIVATE ICUTU)
target_link_libraries(Core PRIVATE ICUUC)
target_link_libraries(Core PRIVATE SDL2)
target_link_libraries(Core PRIVATE FreeImage)
target_link_libraries(Core PRIVATE bc7enc)
target_link_libraries(Core PRIVATE glm)

# Editor headers
file(GLOB_RECURSE EDITOR_HEADERS "Editor/Source/*.h")

# Editor sources
file(GLOB_RECURSE EDITOR_SOURCES "Editor/Source/*.cpp")

# Editor (executable, depends on Core)
add_executable(Editor ${EDITOR_HEADERS} ${EDITOR_SOURCES})

set_target_properties(Editor PROPERTIES OUTPUT_NAME "Editor.app")

set_target_properties(Editor PROPERTIES
    BUILD_RPATH "$ORIGIN"
    INSTALL_RPATH "$ORIGIN"
)

target_include_directories(Editor PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Source
    ${CMAKE_CURRENT_SOURCE_DIR}/Editor/Dependencies/Carve/include
)

add_library(Carve STATIC IMPORTED)
set_target_properties(Carve PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Editor/Dependencies/Carve/lib/linux64/libcarve.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Editor/Dependencies/Carve/include"
)

# Link Editor and Core
target_link_libraries(Editor PRIVATE Core)
target_link_libraries(Editor PRIVATE Carve)

# Group projects
set_property(TARGET Core PROPERTY FOLDER "Core")
set_property(TARGET Editor PROPERTY FOLDER "Editor")

# Output directory
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/linux64)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/linux64)

add_custom_command(
    TARGET Editor
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Resources" "${CMAKE_BINARY_DIR}/linux64/Editor"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/assimp/bin/linux64/libassimp.so.6.0.2" "${CMAKE_BINARY_DIR}/linux64/libassimp.so.6"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/glew/bin/linux64/libGLEW.so.2.2.0" "${CMAKE_BINARY_DIR}/linux64/libGLEW.so.2.2"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/Core/Dependencies/sdl/bin/linux64/libSDL2-2.0.so.0.2600.3" "${CMAKE_BINARY_DIR}/linux64/libSDL2-2.0.so.0"
)