// Taken from Lab 5
// Modified by Steven Smith on 2020-04-04.
// Modified for Concordia W 2020 Comp 371
// A class created to help and simplify the main driver into something more simple
// and easier to manage. A singleton instance that would keep references to important objects such as
// the currently active Shader
//
#include "Renderer.h"
#include "WindowManager.h"
//default constructor

std::vector<Shader*> Renderer::shaderList;
Shader * Renderer::currentShader = nullptr;
GLuint Renderer::renderMode = -1;
void Renderer::Initialize(glm::vec3 color, float point_size) {
    glfwMakeContextCurrent(WindowManager::getWindow());

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");

        getchar();
        exit(-1);
    }

    // Somehow, glewInit triggers a glInvalidEnum... Let's ignore it
    glGetError();

    // Black background
    glClearColor(color.x, color.y, color.z, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(point_size);
    CheckForErrors();
}

void Renderer::Shutdown() {
    // Shaders
    for(int i = 0; i < shaderList.size(); i++){
        glDeleteProgram(shaderList[i]->ID);
        delete shaderList[i];
        shaderList[i] = nullptr;
    }

    currentShader = nullptr;
    shaderList.clear();
    WindowManager::Shutdown();
}
void Renderer::BeginFrame()
{
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Renderer::EndFrame()
{
    // Swap buffers
    glfwSwapBuffers(WindowManager::getWindow());
}


bool Renderer::PrintError()
{
    static bool checkForErrors = true;

    //
    if( !checkForErrors )
    {
        return false;
    }

    //
    const char * errorString = NULL;
    bool retVal = false;

    switch( glGetError() )
    {
        case GL_NO_ERROR:
            retVal = true;
            break;

        case GL_INVALID_ENUM:
            errorString = "GL_INVALID_ENUM";
            break;

        case GL_INVALID_VALUE:
            errorString = "GL_INVALID_VALUE";
            break;

        case GL_INVALID_OPERATION:
            errorString = "GL_INVALID_OPERATION";
            break;

        case GL_INVALID_FRAMEBUFFER_OPERATION:
            errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;

        case GL_OUT_OF_MEMORY:
            errorString = "GL_OUT_OF_MEMORY";
            break;

        default:
            errorString = "UNKNOWN";
            break;
    }

    //
    if( !retVal )
    {
        printf( "%s\n", errorString );
    }

    //
    return retVal;
}


void Renderer::CheckForErrors(){
    while(PrintError() == false)
    {
    }
}

Shader* Renderer::getCurrentShader() {
    return currentShader;
}

void Renderer::useShader(int index) {
    if(index < 0 || index > shaderList.size()){
        std::cout << "ERROR INDEX OUT OF BOUNDS" << std::endl;
        Shutdown();
    }
    currentShader = shaderList[index];
    currentShader->use();
}

void Renderer::addShader(Shader* shader) {
    shaderList.push_back(shader);
    currentShader = shaderList.back();
    currentShader->use();
}

void Renderer::setRenderMode(GLuint mode) {
    renderMode = mode;
}