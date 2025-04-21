cmake_minimum_required(VERSION 3.14)
project(LearningECS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Path to the header files
include_directories(
    ${PROJECT_SOURCE_DIR}/external/glad/include
    ${PROJECT_SOURCE_DIR}/external/glfw/include
    ${PROJECT_SOURCE_DIR}/include
)

# fetch and download glfw
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
FetchContent_MakeAvailable(glfw)

add_executable(LearningECS
    src/main.cpp
    src/glad.c
)

# set OPENGL_LIB to the OS specific openGL library
if(WIN32)
    set(OPENGL_LIB opengl32)
elseif(APPLE)
    find_library(OpenGL_LIBRARY OpenGL)
    set(OPENGL_LIB ${OpenGL_LIBRARY})
elseif(UNIX)
    set(OPENGL_LIB GL)
endif()

# link glfw and openGL
target_link_libraries(LearningECS PRIVATE glfw ${OPENGL_LIB})







