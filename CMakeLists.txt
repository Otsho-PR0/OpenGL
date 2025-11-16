cmake_minimum_required(VERSION 3.16)

project(OpenGL)

find_package(glfw3 CONFIG REQUIRED)
find_package(glew CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

file(GLOB SOURCES src/*.c*)
add_executable(OpenGL ${SOURCES})
target_link_libraries(OpenGL PRIVATE glfw GLEW::GLEW glm::glm assimp::assimp imgui::imgui)
