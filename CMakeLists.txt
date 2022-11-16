cmake_minimum_required(VERSION 3.6)
project(learn-opengl)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

set(SOURCE_FILES src/main.cc)
add_executable(learn-opengl ${SOURCE_FILES})

target_link_libraries(learn-opengl ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw)
