//
// Modified by Steven Smith on 2020-04-04.
// Modified for Concordia W 2020 Comp 371
// A class created to help and simplify the main driver into something more simple
// and easier to manage.
//  A modified version of labs5 EventManager


#include "GLFW/glfw3.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "WindowManager.h"

// set up and define the frame time static variable
double WindowManager::sLastFrameTime = glfwGetTime();
float  WindowManager::sFrameTime = 0.0f;

// Mouse
double WindowManager::sLastMousePositionX = 0.0f;
float  WindowManager::sMouseDeltaX = 0.0f;
double WindowManager::sLastMousePositionY = 0.0f;
float  WindowManager::sMouseDeltaY = 0.0f;

// Window
GLFWwindow* WindowManager::window = nullptr;


void WindowManager::Initialize(std::string title, int width, int height) {
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit(-1);
    }


    // set up context hint based on the platform
#if defined(PLATFORM_OSX) || defined(GLM_PLATFORM_LINUX)
    // OS X would use legacy opengl by default, and wouldn't be able to load shaders
    // This is the proper way to setup GLFW to use modern OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);

#else
    // Allow older laptops to run the framework, even though, our framework
    // is compatible with OpenGL 3.3 (which removed many deprecated features)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_DEPTH_BITS, 32);
#endif


    // Open a window and create its OpenGL context
    glfwWindowHint(GLFW_RESIZABLE, 0);
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (window == nullptr)
    {
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        exit(-1);
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glViewport(0,0, width, height);
    // Initial mouse position
    glfwPollEvents();
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    sLastMousePositionX = x;
    sLastMousePositionY = y;

    // Initial frame time time
    sLastFrameTime = glfwGetTime();
    srand((unsigned int) time(nullptr));
}

void WindowManager::Shutdown() {
    glfwTerminate();
    window = nullptr;
}

void WindowManager::Update()
{
    // Update inputs / events
    glfwPollEvents();

    // Update mouse position and frame time
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    sMouseDeltaX = static_cast<float>( x - sLastMousePositionX );
    sMouseDeltaY = static_cast<float>( y - sLastMousePositionY );
    sLastMousePositionX = x;
    sLastMousePositionY = y;

    double currentTime = glfwGetTime();
    sFrameTime = static_cast<float>(currentTime - sLastFrameTime);
    sLastFrameTime = currentTime;
}

void WindowManager::EnableMouseCursor()
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void WindowManager::DisableMouseCursor()
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

float WindowManager::GetMouseMotionX()
{
    return sMouseDeltaX;
}

float WindowManager::GetMouseMotionY()
{
    return sMouseDeltaY;
}

GLFWwindow* WindowManager::getWindow() {
    return window;
}

float WindowManager::GetFrameTime()
{
    return sFrameTime;
}

bool WindowManager::ExitWindow()
{
    return glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS || glfwWindowShouldClose(window);
}