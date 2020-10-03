cmake_minimum_required(VERSION 3.17)
project(A1)

set(CMAKE_CXX_STANDARD 14)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(A1 main.cpp Framework/Renderer.cpp Framework/WindowManager.cpp Objects/Cell.cpp Objects/Cell.h Objects/Grid.cpp Objects/Grid.h)

set(OpenGlLinkers -lglut -lGLU -lGL) # setting all the Glut libraries as one variable.
target_link_libraries(A1 PRIVATE GLEW::GLEW PRIVATE glfw PRIVATE glm  ${OpenGlLinkers})