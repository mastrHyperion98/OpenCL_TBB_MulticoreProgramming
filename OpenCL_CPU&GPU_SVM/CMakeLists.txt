cmake_minimum_required(VERSION 3.17)
project(A3)

set(CMAKE_CXX_STANDARD 14)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED)
find_package(OpenCL REQUIRED)

add_executable(A3 main.cpp Framework/Renderer.cpp Framework/WindowManager.cpp  Objects/Grid.cpp Objects/Grid.h)

#set(OpenGlLinkers -lglut -lGLU -lGL) # setting all the Glut libraries as one variable.
target_link_libraries(A3 PRIVATE GLEW::GLEW PRIVATE glfw PRIVATE glm PRIVATE TBB::tbb PRIVATE OpenCL::OpenCL)