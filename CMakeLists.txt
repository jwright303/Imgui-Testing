cmake_minimum_required (VERSION 3.1)
project(imgui-testing)

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
#set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glfw3/glfw-3.3.8)
set(IMGUI_SOURCE_DIR "/Users/jack/Personal-Projects/Shared-Libs/imgui/")

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_library(GLFW glfw3 PAtHS "/Users/jack/Personal-Projects/W-Practice/imgui-testing/glfw/glfw-3.3.8/lib-universal")
#set(GLFW_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/glfw-build/src/libglfw3.a)

set(PINCHOT_API_ROOT_DIR "" CACHE FILEPATH "this is a dumb cmake command")
if(PINCHOT_API_ROOT_DIR STREQUAL "")
  message(FATAL_ERROR "PINCHOT_API_ROOT_DIR is empty, please provide path to Pinchot C API root directory")
endif()


set(LINK_LIBS
  ${OPENGL_LIBRARY}
  ${GLFW}
  "-framework Cocoa"
  "-framework IOKit"
  "-framework CoreVideo"
  -ldl
  -lm)

file(GLOB PROJECT_SOURCES
  ${SOURCE_DIR}/*.cpp
  ${IMGUI_SOURCE_DIR}/*.cpp
  ${IMGUI_SOURCE_DIR}/misc/debuggers/imgui.natvis)

include_directories(${IMGUI_SOURCE_DIR})
include_directories(${PINCHOT_API_ROOT_DIR}/examples)
include_directories("/Users/jack/Personal-Projects/W-Practice/imgui-testing/glfw/glfw-3.3.8/include")
include_directories(${PINCHOT_API_ROOT_DIR}/third-party/boost-1.70.0)

# set(GLFW_BUILD_EXAMPLES OFF)
# set(GLFW_BUILD_TESTS OFF)
# set(GLFW_BUILD_DOCS OFF)
# set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF)

add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${LINK_LIBS})

list(APPEND CMAKE_MODULE_PATH ${PINCHOT_API_ROOT_DIR})
# list(APPEND CMAKE_MODULE_PATH ${PINCHOT_API_ROOT_DIR}/third-party/boost-1.70.0)
include(PinchotBuildApplication RESULT_VARIABLE HAVE_PINCHOT_BUILD_APP)

#Could get rid of this since include already throws an error if it cant find the cmake file but adds some clarity
if(NOT HAVE_PINCHOT_BUILD_APP)
  message("PinchotBuildApplication was not found, check PINCHOT_API_ROOT_DIR define")
endif()