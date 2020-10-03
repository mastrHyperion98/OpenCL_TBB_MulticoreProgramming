#include <iostream>
// Import glew and glfw3 ORDER MATTERS
#include "GL/glew.h"   // Include GLEW - OpenGL Extension Wrangler
#include "GLFW/glfw3.h" // GLFW provides a cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs
// Import glm
#import "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" // include this to create transformation matrices
#include "Framework/WindowManager.h"
#include "Framework/Renderer.h"
#include <thread>
#include "random"
#include "Objects/Grid.h"

#define SCREEN_WIDTH 1024.0f
#define SCREEN_HEIGHT 768.0f
#define POINT_SIZE 1.0f
#define GREEN glm::vec3(0.0f,0.8f,102/255.0f)
#define RED glm::vec3(1.0f,0.0f,0.0f)

using namespace std;
using namespace glm;


Grid *grid = new Grid(SCREEN_WIDTH, SCREEN_HEIGHT);
int RandNumGen(int min, int max) {
    if(min < 0){
        min = 0;
    }
    if(max < min){
        max = min;
    }
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(min,max); // dist
    return dist(rng);
}

void changeColor(){
            int i= RandNumGen(0, SCREEN_WIDTH-1);
            int j = RandNumGen(0, SCREEN_HEIGHT-1);
            grid->UpdateColor(i,j,glm::vec3(1.0f, 1.0f,1.0f));
}

void CancerMutationCheck(int start, int end){
    int const MUTATION_THRESHOLD = 5; // errors in slide
    int const MUTATION_THRESHOLD_CORNER = 3;
    int const MUTATION_THRESHOLD_EDGE = 4;
    int width = SCREEN_WIDTH-1;
    int height = SCREEN_HEIGHT-1;
    // traverse every cell one by one and check for mutation....if color is red skip.
    for(int j = start; j < end; j++){
        for(int i = 0; i < width -1; i++){
            if(grid->matrix[i][j]->getColor() == RED)
                continue;
            else{
                int numberCancer = 0;
                // check if corner
                if((i == 0 || i == width) && (j == 0 || j == height)){
                    // there are only four corners i == 0 j == 0
                    if(i==0 && j==0){
                        // only three checks can be done
                        if(grid->matrix[i][j+1]->getColor() == RED)
                            numberCancer++;
                        if(grid->matrix[i+1][j]->getColor() == RED)
                            numberCancer++;
                        if(grid->matrix[i+1][j+1]->getColor() == RED)
                            numberCancer++;

                        if(numberCancer >= MUTATION_THRESHOLD_CORNER)
                            grid->UpdateColor(i,j, RED);
                    }
                    else if(i==width && j==0){
                        if(grid->matrix[i][j+1]->getColor() == RED)
                            numberCancer++;
                        if(grid->matrix[i-1][j]->getColor() == RED)
                            numberCancer++;
                        if(grid->matrix[i-1][j+1]->getColor() == RED)
                            numberCancer++;

                        if(numberCancer >= MUTATION_THRESHOLD_CORNER)
                            grid->UpdateColor(i,j, RED);
                    }
                    else if(j==height && i == width){
                        if(grid->matrix[i][j-1]->getColor() == RED)
                            numberCancer++;
                        if(grid->matrix[i-1][j]->getColor() == RED)
                            numberCancer++;
                        if(grid->matrix[i-1][j-1]->getColor() == RED)
                            numberCancer++;

                        if(numberCancer >= MUTATION_THRESHOLD_CORNER)
                            grid->UpdateColor(i,j, RED);
                    }
                    else{
                        if(grid->matrix[i][j-1]->getColor() == RED)
                            numberCancer++;
                        if(grid->matrix[i+1][j]->getColor() == RED)
                            numberCancer++;
                        if(grid->matrix[i+1][j-1]->getColor() == RED)
                            numberCancer++;

                        if(numberCancer >= MUTATION_THRESHOLD_CORNER)
                            grid->UpdateColor(i,j, RED);
                        continue;
                    }
                }
                // check if edge
                // is edge if i == 0 or width or if j == 0 or height
                else if(i == 0 || i == width || j == 0 or j == height){
                    // only four options is left, bottom, right or top
                    if(j == 0){
                        // cannot check left so no - j
                        if(grid->matrix[i-1][j]->getColor() == RED) // up
                            numberCancer++;
                        if(grid->matrix[i-1][j+1]->getColor() == RED) // up-right
                            numberCancer++;
                        if(grid->matrix[i][j+1]->getColor() == RED) // right
                            numberCancer++;
                        if(grid->matrix[i+1][j+1]->getColor() == RED) // bottom right
                            numberCancer++;
                        if(grid->matrix[i+1][j]->getColor() == RED) // bottom
                            numberCancer++;

                        if(numberCancer >= MUTATION_THRESHOLD_CORNER)
                            grid->UpdateColor(i,j, RED);
                    }
                    else if(i == width){
                        if(grid->matrix[i-1][j]->getColor() == RED) // up
                            numberCancer++;
                        if(grid->matrix[i-1][j+1]->getColor() == RED) // up-right
                            numberCancer++;
                        if(grid->matrix[i][j+1]->getColor() == RED) // right
                            numberCancer++;
                        if(grid->matrix[i][j-1]->getColor() == RED) // bottom lleft
                            numberCancer++;
                        if(grid->matrix[i-1][j-1]->getColor() == RED) // top left
                            numberCancer++;

                        if(numberCancer >= MUTATION_THRESHOLD_CORNER)
                            grid->UpdateColor(i,j, RED);
                    }
                    else if(j==height){
                        if(grid->matrix[i-1][j]->getColor() == RED) // up
                            numberCancer++;
                        if(grid->matrix[i-1][j-1]->getColor() == RED) // up-left
                            numberCancer++;
                        if(grid->matrix[i][j-1]->getColor() == RED) // left
                            numberCancer++;
                        if(grid->matrix[i+1][j-1]->getColor() == RED) // bottom left
                            numberCancer++;
                        if(grid->matrix[i+1][j]->getColor() == RED) // bottom
                            numberCancer++;

                        if(numberCancer >= MUTATION_THRESHOLD_CORNER)
                            grid->UpdateColor(i,j, RED);
                    }
                    else{
                        if(grid->matrix[i+1][j]->getColor() == RED) // bottom
                            numberCancer++;
                        if(grid->matrix[i+1][j+1]->getColor() == RED) // bottom-right
                            numberCancer++;
                        if(grid->matrix[i][j+1]->getColor() == RED) // right
                            numberCancer++;
                        if(grid->matrix[i][j-1]->getColor() == RED) // left
                            numberCancer++;
                        if(grid->matrix[i+1][j-1]->getColor() == RED) // bottom left
                            numberCancer++;

                        if(numberCancer >= MUTATION_THRESHOLD_CORNER)
                            grid->UpdateColor(i,j, RED);
                    }

                }
                // else regular threshold
                // all other regular stuff -- here
            }
        }
    }
}

int main() {
    // WindowManager and Renderer are classes from my Comp 371 Project that I am re-using here for quick startup/clean code.
    // Both of these classes were written/converted by me and I can prove it through GitHub if necessary!
    // Shader.h is borrowed from LearnOpenGL.com

    WindowManager::Initialize("Multicore Assignment1", SCREEN_WIDTH, SCREEN_HEIGHT);
    Renderer::Initialize(vec3(0,0.5,0.5), POINT_SIZE);
    // add our shaders
    Renderer::addShader(new Shader("../Shaders/vertex_shader.glsl","../Shaders/frag_shader.glsl"));
    Renderer::setRenderMode(GL_POINTS);
    Renderer::useShader(0);

    glm::mat4 projection = glm::ortho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);
    Renderer::getCurrentShader()->setMat4("orthoMatrix", projection);
    grid->CreateVAO();
    const auto processor_count = std::thread::hardware_concurrency() - 2; // processor count - 2
    int indices_per_thread = (SCREEN_HEIGHT-1)/processor_count;
   do{
        WindowManager::Update();
        // Update World
        float dt = WindowManager::GetFrameTime();
        Renderer::BeginFrame();
        // set the color
       thread threadList[processor_count];
       for(int i = 0; i < processor_count; i++){
           threadList[i] = thread(CancerMutationCheck, i * indices_per_thread ,
                                  ((i+1)*indices_per_thread <= SCREEN_HEIGHT-1)?(i+1)*indices_per_thread:(SCREEN_HEIGHT-1));
       }
       for(int i = 0; i < processor_count; i++){
          threadList[i].join();
       }
       grid->UpdateVAO();
       grid->Draw();
       Renderer::EndFrame();
    }while(!WindowManager::ExitWindow());
    //shutdown the renderer
    Renderer::Shutdown();
   delete grid;
}
