cmake_minimum_required(VERSION 3.20)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# Project
project(rubik-cube
    VERSION 1.0
    DESCRIPTION "Interactive 3D Rubik's Cube with OpenGL"
    LANGUAGES C CXX
)

# Global Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Defaults build type
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    message(STATUS "No build type specified, Defaulting to Release.")
endif()

# Compiler flags
if(MSVC)
    add_compile_options(
        /W4
        /permissive-
        /Zc:__cplusplus
        /MP
        /utf-8
    )

    # Debug
    add_compile_options("$<$<CONFIG:Debug>:/Od>")
    add_compile_options("$<$<CONFIG:Debug>:/Zi>")
    add_compile_options("$<$<CONFIG:Debug>:/RTC1>")
    add_compile_options("$<$<CONFIG:Debug>:/MDd>")

    # Release
    add_compile_options("$<$<CONFIG:Release>:/O2>")
    add_compile_options("$<$<CONFIG:Release>:/MD>")
    add_compile_options("$<$<CONFIG:Release>:/DNDEBUG>")

    # Linker
    add_link_options("$<$<CONFIG:Debug>:/DEBUG>")
    add_link_options("$<$<CONFIG:Release>:/INCREMENTAL:NO>")
else()
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wnon-virtual-dtor
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wconversion
        -Wsign-conversion
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2
    )

    # Debug
    add_compile_options("$<$<CONFIG:Debug>:-O0>")
    add_compile_options("$<$<CONFIG:Debug>:-g3>")

    # Release
    add_compile_options("$<$<CONFIG:Release>:-O3>")
    add_compile_options("$<$<CONFIG:Release>:-DNDEBUG>")
endif()

# Dependencies
include(FetchContent)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# GLW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    GIT_SHALLOW TRUE
)

# GLW
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.36
    GIT_SHALLOW TRUE
)

# Make dependencies available
FetchContent_MakeAvailable(glfw glm)

# GLAD
add_subdirectory(cmake)

# Main executable
add_executable(rubik-cube
    src/main.cpp
    src/app/Application.cpp
    src/core/Cube.cpp
    src/core/RubiksCube.cpp
    src/render/Window.cpp
    src/render/Renderer.cpp
    src/render/Shader.cpp
    src/render/Mesh.cpp
    src/render/Camera.cpp
    src/core/Cube.cpp
)

# Include directories
target_include_directories(rubik-cube
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/include
    ${glm_SOURCE_DIR}
)

# Link Libraries
target_link_libraries(rubik-cube
    PRIVATE
    glfw
    glad
)

# Compile definitions
target_compile_definitions(rubik-cube
    PRIVATE
        GLFW_INCLUDE_NONE
)

# Copy Assets
add_custom_command(TARGET rubik-cube POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:rubik-cube>/assets
    COMMENT "Copying assets to build directory"
)

# Messages
message(STATUS "")
message(STATUS "════════════════════════════════════════════")
message(STATUS "    rubik-cube configuration summary")
message(STATUS "════════════════════════════════════════════")
message(STATUS "    Build Type      : ${CMAKE_BUILD_TYPE}")
message(STATUS "    C++ standard    : ${CMAKE_CXX_STANDARD}")
message(STATUS "    Compiler        : ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "    Generator       : ${CMAKE_GENERATOR}")
message(STATUS "════════════════════════════════════════════")
message(STATUS "")
