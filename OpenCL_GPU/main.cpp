#define CL_HPP_ENABLE_EXCEPTIONS
#include <iostream>
// Import glew and glfw3 ORDER MATTERS
#include "GL/glew.h"   // Include GLEW - OpenGL Extension Wrangler
#include "GLFW/glfw3.h" // GLFW provides a cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs
#include "glm/gtc/matrix_transform.hpp" // include this to create transformation matrices
#include "Framework/WindowManager.h"
#include "Framework/Renderer.h"
#include "random"
#include "Objects/Grid.h"
#include <deque>
#include "Framework/CL_Tools.h"

#define SCREEN_WIDTH 1024.0f
#define SCREEN_HEIGHT 768.0f
#define POINT_SIZE 1.0f
#define PROGRAM "../Kernels/gpu_kernels.cl"

using namespace std;

Grid *grid = new Grid(SCREEN_WIDTH, SCREEN_HEIGHT);
int const MUTATION_THRESHOLD = 6; // errors in slide
int const MUTATION_THRESHOLD_CORNER = 3;
int const MUTATION_THRESHOLD_EDGE = 4;
int const X_MAX = SCREEN_WIDTH-1;
int const Y_MAX = SCREEN_HEIGHT-1;

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
// check if a cancer cell is to be turned into a healthy cell/


void Injection(){
    // select a point of injection
    int i =  RandNumGen(0, X_MAX);
    int j = RandNumGen(0, Y_MAX);
    int threshold = RandNumGen(0,9);
    // x and y is our point of injection
    // determine if x and y is in a corner, edge or normal
    // based on position randomly decide whether or not a cell becomes  an medicine cell or not
    /**
     * This algorithm is going to be very similar to the ones for CancerCheck and HealthyCheck were we look at the neighbouring
     * cells. However, a random number is generated to determine the threshold for eligibility. At least one cell will be
     * converted into a medicine cell. However, more than one will be left up to the programs random properties.
     */

    int numberMedical = 0;
    // check if corner
    if ((i == 0 || i == X_MAX) && (j == 0 || j == Y_MAX)) {
        // there are only four corners i == 0 j == 0
        if (i == 0 && j == 0) { // top left corner
            // only three checks can be done
            if (RandNumGen(0,10) > threshold){// down
                numberMedical++;
                int index = ((i*Y_MAX)+(j+1));
                grid->directions[index] = DOWN;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i,j+1)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i, j + 1, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold){// right
                numberMedical++;
                grid->directions[((i+1)*Y_MAX)+(j)] = RIGHT;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i+1,j)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i+1, j, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold || numberMedical == 0){// down-right
                int index = (((i+1)*Y_MAX)+(j-1));
                grid->directions[index] = DOWN_RIGHT;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i+1,j-1)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i+1, j - 1, YELLOW);
                    grid->IncremementMedical();
                }
            }
        } else if (i == X_MAX && j == 0) { // top right
            if (RandNumGen(0,10) > threshold){// down
                numberMedical++;
                int index = (((i)*Y_MAX)+(j+1));
                grid->directions[index] = DOWN;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i,j+1)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i, j + 1, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold){// left
                numberMedical++;
                int index = (((i-1)*Y_MAX)+(j));
                grid->directions[index] = LEFT;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i-1,j)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i-1, j, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold || numberMedical==0){// left-down
                int index = (((i-1)*Y_MAX)+(j+1));
                grid->directions[index] = DOWN_LEFT;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i-1,j+1)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i-1, j + 1, YELLOW);
                    grid->IncremementMedical();
                }
            }
        } else if (j == Y_MAX && i == X_MAX) { // bottom right
            if (RandNumGen(0,10) > threshold){// up
                numberMedical++;
                int index = (((i)*Y_MAX)+(j-1));
                grid->directions[index] = UP;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i,j-1)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i, j - 1, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold){// left
                numberMedical++;
                int index = (((i-1)*Y_MAX)+(j));
                grid->directions[index] = LEFT;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i-1,j)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i-1, j, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold || numberMedical == 0){// up-left
                int index = (((i-1)*Y_MAX)+(j-1));
                grid->directions[index] = UP_LEFT;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i-1,j-1)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i-1, j- 1, YELLOW);
                    grid->IncremementMedical();
                }
            }
        } else { // bottom left
            if (RandNumGen(0,10) > threshold){ // up
                numberMedical++;
                int index = (((i)*Y_MAX)+(j-1));
                grid->directions[index] = UP;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i,j-1)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i, j - 1, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold){// right
                numberMedical++;
                int index = (((i+1)*Y_MAX)+(j));
                grid->directions[index] = RIGHT;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i+1,j)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i+1, j, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold || numberMedical == 0){// up-right
                int index = (((i+1)*Y_MAX)+(j-1));
                grid->directions[index] = UP_RIGHT;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i+1,j-1)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i+1, j - 1, YELLOW);
                    grid->IncremementMedical();
                }
            }
        }
    }
        // check if edge
        // is edge if i == 0 or width or if j == 0 or height
    else if (i == 0 || i == X_MAX || j == 0 || j == Y_MAX) {
        // only four options is left, bottom, right or top
        if (j == 0) {
            // top row
            if (RandNumGen(0,10) > threshold){// left
                numberMedical++;
                int index = (((i-1)*Y_MAX)+(j));
                grid->directions[index] = LEFT;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i-1,j)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i-1, j, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold){// bottom-left
                numberMedical++;
                int index = (((i-1)*Y_MAX)+(j+1));
                grid->directions[index] = DOWN_LEFT;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i-1,j+1)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i-1, j + 1, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold){// right
                numberMedical++;
                int index = (((i+1)*Y_MAX)+(j));
                grid->directions[index] = RIGHT;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i+1,j)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i+1, j, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold){// bottom right
                numberMedical++;
                int index = (((i+1)*Y_MAX)+(j+1));
                grid->directions[index] = DOWN_RIGHT;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i+1,j+1)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i+1, j + 1, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold || numberMedical == 0){// bottom
                int index = (((i)*Y_MAX)+(j+1));
                grid->directions[index] = DOWN;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i,j+1)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i, j + 1, YELLOW);
                    grid->IncremementMedical();
                }
            }
        } else if (i == X_MAX) { // right side
            if (RandNumGen(0,10) > threshold){// up
                numberMedical++;
                int index = (((i)*Y_MAX)+(j-1));
                grid->directions[index] = UP;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i,j-1)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i, j - 1, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold){// up-left
                numberMedical++;
                int index = (((i-1)*Y_MAX)+(j-1));
                grid->directions[index] = UP_LEFT;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i-1,j-1)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i-1, j - 1, YELLOW);
                    grid->IncremementMedical();
                }

            }
            if (RandNumGen(0,10) > threshold){// left
                numberMedical++;
                int index = (((i-1)*Y_MAX)+(j));
                grid->directions[index] = LEFT;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i-1,j)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i-1, j, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold){// bottom lleft
                numberMedical++;
                int index = (((i-1)*Y_MAX)+(j+1));
                grid->directions[index] = DOWN_LEFT;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i-1,j+1)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i-1, j + 1, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold || numberMedical){// bottom
                numberMedical++;
                int index = (((i)*Y_MAX)+(j+1));
                grid->directions[index] = DOWN;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i,j+1)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i, j + 1, YELLOW);
                    grid->IncremementMedical();
                }
            }
        } else if (j == Y_MAX) { // bottom row
            if (RandNumGen(0,10) > threshold){// up
                numberMedical++;
                int index = (((i)*Y_MAX)+(j-1));
                grid->directions[index] = UP;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i,j-1)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i, j - 1, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold){// up-left
                numberMedical++;
                int index = (((i-1)*Y_MAX)+(j-1));
                grid->directions[index] = UP_LEFT;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i-1,j-1)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i-1, j - 1, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold){// left
                numberMedical++;
                int index = (((i-1)*Y_MAX)+(j));
                grid->directions[index] = LEFT;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i-1,j)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i-1, j, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold){// up right
                numberMedical++;
                int index = (((i+1)*Y_MAX)+(j-1));
                grid->directions[index] = UP_RIGHT;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i+1,j-1)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i+1, j - 1, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold || numberMedical==0){// right
                numberMedical++;
                int index = (((i+1)*Y_MAX)+(j));
                grid->directions[index] = RIGHT;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i+1,j)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i+1, j, YELLOW);
                    grid->IncremementMedical();
                }
            }
        } else { // left side
            if (RandNumGen(0,10) > threshold){// bottom
                numberMedical++;
                int index = (((i)*Y_MAX)+(j+1));
                grid->directions[index] = DOWN;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i,j+1)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i, j + 1, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold){// bottom-right
                numberMedical++;
                int index = (((i+1)*Y_MAX)+(j+1));
                grid->directions[index] = DOWN_RIGHT;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i+1,j+1)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i+1, j + 1, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold){// right
                numberMedical++;
                int index = (((i+1)*Y_MAX)+(j));
                grid->directions[index] = RIGHT;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i+1,j)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i+1, j, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold){// U{P
                numberMedical++;
                int index = (((i)*Y_MAX)+(j-1));
                grid->directions[index] = UP;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i,j-1)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i, j - 1, YELLOW);
                    grid->IncremementMedical();
                }
            }
            if (RandNumGen(0,10) > threshold || numberMedical==0){// UP_RIGHT
                int index = (((i+1)*Y_MAX)+(j-1));
                grid->directions[index] = UP_RIGHT;
                cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i+1,j-1)];
                if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                    grid->UpdateColor(i+1, j - 1, YELLOW);
                    grid->IncremementMedical();
                }
            }
        }
    }
    else {
        // else regular threshold
        // all other regular stuff -- here
        // check up
        if (RandNumGen(0,10) > threshold){// bottom
            numberMedical++;
            int index = (((i)*Y_MAX)+(j-1));
            grid->directions[index] = UP;
            cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i,j-1)];
            if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                grid->UpdateColor(i, j - 1, YELLOW);
                grid->IncremementMedical();
            }
        }
        // check top-left
        if (RandNumGen(0,10) > threshold){// bottom
            numberMedical++;
            int index = (((i-1)*Y_MAX)+(j-1));
            grid->directions[index] = UP_LEFT;
            cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i-1,j-1)];
            if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                grid->UpdateColor(i-1, j - 1, YELLOW);
                grid->IncremementMedical();
            }
        }
        // check top-right
        if (RandNumGen(0,10) > threshold){// bottom
            numberMedical++;
            int index = (((i+1)*Y_MAX)+(j-1));
            grid->directions[index] = UP_RIGHT;
            cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i+1,j-1)];
            if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                grid->UpdateColor(i+1, j - 1, YELLOW);
                grid->IncremementMedical();
            }
        }
        // check left
        if (RandNumGen(0,10) > threshold){// bottom
            numberMedical++;
            int index = (((i-1)*Y_MAX)+(j));
            grid->directions[index] = LEFT;
            cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i-1,j)];
            if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                grid->UpdateColor(i-1, j, YELLOW);
                grid->IncremementMedical();
            }
        }
        // check right
        if (RandNumGen(0,10) > threshold){// bottom
            numberMedical++;
            int index = (((i+1)*Y_MAX)+(j));
            grid->directions[index] = RIGHT;
            cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i+1,j)];
            if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                grid->UpdateColor(i+1, j, YELLOW);
                grid->IncremementMedical();
            }
        }
        // check bottom-left
        if (RandNumGen(0,10) > threshold){// bottom
            numberMedical++;
            int index = (((i-1)*Y_MAX)+(j+1));
            grid->directions[index] = DOWN_LEFT;
            cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i-1,j+1)];
            if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                grid->UpdateColor(i-1, j + 1, YELLOW);
                grid->IncremementMedical();
            }
        }
        // check bottom
        if (RandNumGen(0,10) > threshold){// bottom
            numberMedical++;
            int index = (((i)*Y_MAX)+(j+1));
            grid->directions[index] = DOWN;
            cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i,j+1)];
            if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                grid->UpdateColor(i, j + 1, YELLOW);
                grid->IncremementMedical();
            }
        }
        // check bottom-right
        if (RandNumGen(0,10) > threshold || numberMedical == 0) {// bottom
            int index = (((i+1)*Y_MAX)+(j+1));
            grid->directions[index] = DOWN_RIGHT;
            cl_float3 color = grid->vectorArray[grid->getVaoColorIndex(i+1,j+1)];
            if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
                grid->UpdateColor(i+1, j + 1, YELLOW);
                grid->IncremementMedical();
            }
        }
    }
}

int main() {
    // WindowManager and Renderer are classes from my Comp 371 Project that I am re-using here for quick startup/clean code.
    // Both of these classes were written/converted by me and I can prove it through GitHub if necessary!
    // Shader.h is borrowed from LearnOpenGL.com
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    float const REFRESH_TIME = 1/30.0f;
    int const SIZE = (Y_MAX*X_MAX*2);
    WindowManager::Initialize("Multicore Assignment1", SCREEN_WIDTH*POINT_SIZE, SCREEN_HEIGHT*POINT_SIZE);
    Renderer::Initialize(glm::vec3(0,0.5,0.5), POINT_SIZE);
    // add our shaders
    Renderer::addShader(new Shader("../Shaders/vertex_shader.glsl","../Shaders/frag_shader.glsl"));
    Renderer::setRenderMode(GL_POINTS);
    Renderer::useShader(0);

    cl::Device gpu= GetDeviceTarget(CL_DEVICE_TYPE_GPU);
    cl::Context context = cl::Context(gpu);
    cl::Program program = buildProgram(context, gpu, PROGRAM);
    //create queue to which we will push commands for the device.
    cl::CommandQueue queue(context,gpu);

    glm::mat4 projection = glm::ortho(0.0f, SCREEN_WIDTH*POINT_SIZE, SCREEN_HEIGHT*POINT_SIZE, 0.0f, -1.0f, 1.0f);
    Renderer::getCurrentShader()->setMat4("orthoMatrix", projection);
    cl::Buffer buffer_vectorArray(context,CL_MEM_READ_WRITE,sizeof(cl_float3)*SIZE);
    cl::Buffer buffer_numberCancer(context,CL_MEM_READ_WRITE,sizeof(int));
    cl::Buffer buffer_numberMedical(context,CL_MEM_READ_WRITE,sizeof(int));
    cl::Buffer bufferResult(context, CL_MEM_READ_WRITE, sizeof(cl_float3) * SIZE);
    cl::Buffer bufferDirection(context, CL_MEM_READ_WRITE, sizeof(int) * (Y_MAX*X_MAX));
    cl::Buffer bufferDirectionResult(context, CL_MEM_READ_WRITE, sizeof(int) * (Y_MAX*X_MAX));
    queue.enqueueWriteBuffer(bufferDirectionResult, CL_TRUE, 0, sizeof(int)*(Y_MAX*X_MAX), grid->directions);
    queue.enqueueWriteBuffer(bufferResult,CL_TRUE,0,sizeof(cl_float3)*SIZE, grid->vectorArray);
    queue.enqueueWriteBuffer(buffer_numberCancer,CL_TRUE,0,sizeof(int)*1, grid->numCancer);
    queue.enqueueWriteBuffer(buffer_vectorArray,CL_TRUE,0,sizeof(cl_float3)*SIZE, grid->vectorArray);
    queue.enqueueWriteBuffer(buffer_numberMedical,CL_TRUE,0,sizeof(int)*1, grid->numMedical);
    cl::Kernel mutation=cl::Kernel(program,"mutation_kernel");
    //cl::Kernel cancer_check=cl::Kernel(program,"cancer_check");
    //cl::Kernel radial_expansion=cl::Kernel(program,"radial_expansion");
    mutation.setArg(0,buffer_vectorArray);
    mutation.setArg(1, X_MAX);
    mutation.setArg(2, Y_MAX);
    mutation.setArg(3,buffer_numberCancer);
    mutation.setArg(4,buffer_numberMedical);
    grid->CreateVAO();
    float timer=0;
   do{
        WindowManager::Update();
        // Update World
        Renderer::BeginFrame();
        // set the color
        if(timer >= REFRESH_TIME) {
            //queue.enqueueWriteBuffer(bufferDirection, CL_TRUE, 0, sizeof(int)*(Y_MAX*X_MAX), grid->directions);
            queue.enqueueNDRangeKernel(mutation,cl::NullRange,cl::NDRange(X_MAX, Y_MAX),cl::NullRange);
            queue.finish();
            queue.enqueueReadBuffer(buffer_vectorArray,CL_TRUE,0,sizeof(cl_float3)*SIZE,grid->vectorArray);
            queue.enqueueReadBuffer(buffer_numberCancer, CL_TRUE, 0, sizeof(int), grid->numCancer);
            queue.enqueueReadBuffer(buffer_numberMedical, CL_TRUE, 0, sizeof(int), grid->numMedical);
            grid->UpdateVAO();
            cout << "\nCancer Cells: " << grid->getNumCancer()<< endl;
            cout << "Medical Cells: " << grid->getNumMedical() << endl;
            cout << "Healthy Cells: " << ((Y_MAX*X_MAX)-grid->getNumMedical() - grid->getNumCancer()) << endl;
        }else{
            timer += WindowManager::GetFrameTime();
        }
        cout << "Time since last frame: " << WindowManager::GetFrameTime() << endl;
        grid->Draw();
        Renderer::EndFrame();
   }while(!WindowManager::ExitWindow());
    //shutdown the renderer
     Renderer::Shutdown();
     delete grid;
}
