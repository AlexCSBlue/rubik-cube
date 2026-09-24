# GLAD library
set(GLAD_DIR "${CMAKE_SOURCE_DIR}/third_party/glad")

# Check that vendored GLAD files exist
if(NOT EXISTS "${GLAD_DIR}/src/glad.c")
    message(FATAL_ERROR
        "GLAD source files not found at ${GLAD_DIR}/src/glad.c\n"
        "See docs/GLAD_SETUP.md or run the vendoring script"
    )
endif()

# Create static library for GLAD
add_library(glad STATIC
    ${GLAD_DIR}/src/glad.c
)

# GLAD include directories
target_include_directories(glad
    PUBLIC
        ${GLAD_DIR}/include
)

# GLAD needs to link against OpenGL
if(WIN32)
    target_link_libraries(glad PUBLIC opengl32)
elseif(APPLE)
    find_library(OpenGL_FRAMEWORK OpenGL)
    target_link_libraries(glad PUBLIC ${OpenGL_FRAMEWORK})
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(glad PUBLIC OpenGL::GL)
endif()

# Message
message(STATUS "GLAD configured from ${GLAD_DIR}")
