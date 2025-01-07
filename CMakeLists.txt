cmake_minimum_required(VERSION 3.30)

project(sal)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

include_directories("vendor/glad/include" "vendor/glm" "vendor/stb")

add_library(${PROJECT_NAME} SHARED ${SRC_FILES} "vendor/glad/src/glad.c")

if (APPLE)
    target_include_directories(${PROJECT_NAME} PUBLIC "/opt/homebrew/include")
    target_link_directories(${PROJECT_NAME} PUBLIC "/opt/homebrew/lib")
    target_link_libraries(${PROJECT_NAME} "SDL2" "-framework OpenGL")
endif()
