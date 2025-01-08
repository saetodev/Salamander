cmake_minimum_required(VERSION 3.30)

project(Pong)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

include_directories("../../include" "../../vendor/glm" "../../vendor/glad/include" "../../vendor/stb")
link_directories("../../build")

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} "sal")
