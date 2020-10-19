//
// Created by Steven Smith on 2020-04-04.
// A class created to help and simplify the main driver into something more simple
// and easier to manage. A singleton instance that would keep references to important objects such as
// the currently active Shader
//

#ifndef INC_371PROCEDURALFOREST_RENDERER_H
#define INC_371PROCEDURALFOREST_RENDERER_H
// Include GLEW - OpenGL Extension Wrangler
//#define GLEW_STATIC 1
#include "GL/glew.h"
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "Shader.h"
class Renderer
{
public:
    static void Initialize(glm::vec3 color, float point_size);
    static void Shutdown();
    static void BeginFrame();
    static void EndFrame();
    static Shader* getCurrentShader();
    static void useShader(int index);
    static void addShader(Shader*);
    static int getRenderMode(){return renderMode;};
    static void setRenderMode(GLuint);
private:
    static void CheckForErrors();
    static bool PrintError();
    static std::vector<Shader*> shaderList;
    static Shader *currentShader;
    static GLuint renderMode;
};


#endif //INC_371PROCEDURALFOREST_RENDERER_H
