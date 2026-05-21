cmake_minimum_required(VERSION 4.0)

set(CMAKE_CXX_STANDARD 17)

project(OpenGL)

set(SDL_TEST OFF)
add_subdirectory(thirdparty/SDL/)

set(ASSIMP_BUILD_SHARED_LIBS OFF)
set(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(thirdparty/assimp/)

file(GLOB_RECURSE SOURCES src/*.c*)
file(GLOB IMGUI thirdparty/imgui/*.c*)
add_executable(OpenGL ${SOURCES} ${IMGUI} thirdparty/imgui/backends/imgui_impl_sdl3.cpp thirdparty/imgui/backends/imgui_impl_opengl3.cpp)
target_include_directories(OpenGL PRIVATE include/ thirdparty/SDL/include/ thirdparty/assimp/include/ thirdparty/glad/include/ thirdparty/glm/ thirdparty/imgui/ thirdparty/stb/)
target_link_libraries(OpenGL PRIVATE SDL3::SDL3 assimp)

if(WIN32)
	add_custom_command(TARGET OpenGL POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"$<TARGET_FILE:SDL3::SDL3>"
		"$<TARGET_FILE_DIR:OpenGL>"
		VERBATIM
	)
	add_custom_command(TARGET OpenGL POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"$<TARGET_FILE:assimp>"
		"$<TARGET_FILE_DIR:OpenGL>"
		VERBATIM
	)
endif()