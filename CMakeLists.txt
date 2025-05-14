cmake_minimum_required(VERSION 3.14)
project(LearningECS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)

file(GLOB_RECURSE HEADER_FILES
    ${PROJECT_SOURCE_DIR}/src/*.h
    ${PROJECT_SOURCE_DIR}/src/*.hpp
)
file(GLOB_RECURSE SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/*.c
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)
file(GLOB_RECURSE ASSET_FILES
    ${PROJECT_SOURCE_DIR}/assets/*
)



source_group(TREE ${PROJECT_SOURCE_DIR}/src PREFIX "Header Files" FILES ${HEADER_FILES})
source_group(TREE ${PROJECT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${SOURCE_FILES})
source_group(TREE ${PROJECT_SOURCE_DIR}/assets PREFIX "Assets" FILES ${ASSET_FILES})

add_executable(LearningECS ${HEADER_FILES} ${SOURCE_FILES} ${ASSET_FILES})


target_include_directories(LearningECS PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)


add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/assets
    $<IF:$<CONFIG:Debug>,${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/assets,${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/assets>
    COMMENT "Copying assets to output directory"
)

add_dependencies(LearningECS copy_assets)

set_property(TARGET LearningECS PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>")



# fetch and download glfw
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
FetchContent_MakeAvailable(glfw)

# fetch and download glm
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)
FetchContent_MakeAvailable(glm)


# fetch and download Bullet
FetchContent_Declare(
  bullet
  GIT_REPOSITORY https://github.com/bulletphysics/bullet3.git
  GIT_TAG        master
)

# disable demos and gui builds
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_CPU_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(INSTALL_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(bullet)


# set OPENGL_LIB to the OS specific openGL library
if(WIN32)
    set(OPENGL_LIB opengl32)
elseif(APPLE)
    find_library(OpenGL_LIBRARY OpenGL)
    set(OPENGL_LIB ${OpenGL_LIBRARY})
elseif(UNIX)
    set(OPENGL_LIB GL)
endif()


include_directories(
    ${PROJECT_SOURCE_DIR}/external/glad/include
    ${PROJECT_SOURCE_DIR}/external/glfw/include
    ${bullet_SOURCE_DIR}/src
    ${bullet_SOURCE_DIR}/src/bullet
)

target_link_libraries(LearningECS PRIVATE
    glfw 
    ${OPENGL_LIB} 
    glm::glm
    BulletDynamics
    BulletCollision
    LinearMath
)


