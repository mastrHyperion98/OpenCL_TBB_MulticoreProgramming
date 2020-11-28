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


int const MUTATION_THRESHOLD = 6; // errors in slide
int const MUTATION_THRESHOLD_CORNER = 3;
int const MUTATION_THRESHOLD_EDGE = 4;
int const X_MAX = SCREEN_WIDTH-1;
int const Y_MAX = SCREEN_HEIGHT-1;

Grid *grid;
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

void PrintOutput(Grid *grid){
    cout << "Cancer Cells: " << *grid->numCancer << endl;
    cout << "Medical Cells: " << *grid->numMedical << endl;
    cout << "Healthy Cells: " << ((Y_MAX*X_MAX)-*grid->numMedical - *grid->numCancer) << "\n" << endl;
}

void InjectionKernel(void *args){
    int i =  RandNumGen(0, X_MAX);
    int j = RandNumGen(0, Y_MAX);
    int threshold = RandNumGen(0,9);
    bool top_edge = j == 0;
    bool left_edge = i == 0;
    bool right_edge = i == X_MAX;
    bool bottom_edge = j == Y_MAX;

    if(!top_edge && RandNumGen(0,10) > threshold){
        int x = i;
        int y = j - 1;
        int directionIndex = (x*Y_MAX) + y;
        int colorIndex = 2 * ((x*Y_MAX) + y) + 1;
        grid->directions[directionIndex] = UP;
        cl_float3 color = grid->vectorArray[colorIndex];
        if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
            grid->vectorArray[colorIndex] = YELLOW;
            (*grid->numMedical)++;
        }
    }
    // check top left
    if(!top_edge && !left_edge && RandNumGen(0,10) > threshold){
        int x = i-1;
        int y = j - 1;
        int directionIndex = (x*Y_MAX) + y;
        int colorIndex = 2 * ((x*Y_MAX) + y) + 1;
        grid->directions[directionIndex] = UP_LEFT;
        cl_float3 color = grid->vectorArray[colorIndex];
        if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
            grid->vectorArray[colorIndex] = YELLOW;
            (*grid->numMedical)++;
        }
    }
    // check top right
    if(!top_edge && !right_edge && RandNumGen(0,10) > threshold){
        int x = i+1;
        int y = j - 1;
        int directionIndex = (x*Y_MAX) + y;
        int colorIndex = 2 * ((x*Y_MAX) + y) + 1;
        grid->directions[directionIndex] = UP_RIGHT;
        cl_float3 color = grid->vectorArray[colorIndex];
        if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
            grid->vectorArray[colorIndex] = YELLOW;
            (*grid->numMedical)++;
        }
    }
    // check bottom cell
    if(!bottom_edge && RandNumGen(0,10) > threshold){
        int x = i;
        int y = j + 1;
        int directionIndex = (x*Y_MAX) + y;
        int colorIndex = 2 * ((x*Y_MAX) + y) + 1;
        grid->directions[directionIndex] = DOWN;
        cl_float3 color = grid->vectorArray[colorIndex];
        if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
            grid->vectorArray[colorIndex] = YELLOW;
            (*grid->numMedical)++;
        }
    }
    // check left cell
    if(!left_edge && RandNumGen(0,10) > threshold){
        int x = i-1;
        int y = j;
        int directionIndex = (x*Y_MAX) + y;
        int colorIndex = 2 * ((x*Y_MAX) + y) + 1;
        grid->directions[directionIndex] = LEFT;
        cl_float3 color = grid->vectorArray[colorIndex];
        if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
            grid->vectorArray[colorIndex] = YELLOW;
            (*grid->numMedical)++;
        }
    }
    // check right cell
    if(!right_edge && RandNumGen(0,10) > threshold){
        int x = i+1;
        int y = j;
        int directionIndex = (x*Y_MAX) + y;
        int colorIndex = 2 * ((x*Y_MAX) + y) + 1;
        grid->directions[directionIndex] = RIGHT;
        cl_float3 color = grid->vectorArray[colorIndex];
        if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
            grid->vectorArray[colorIndex] = YELLOW;
            (*grid->numMedical)++;
        }
    }
    // check bottom left
    if(!bottom_edge && !left_edge && RandNumGen(0,10) > threshold){
        int x = i-1;
        int y = j + 1;
        int directionIndex = (x*Y_MAX) + y;
        int colorIndex = 2 * ((x*Y_MAX) + y) + 1;
        grid->directions[directionIndex] = DOWN_LEFT;
        cl_float3 color = grid->vectorArray[colorIndex];
        if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
            grid->vectorArray[colorIndex] = YELLOW;
            (*grid->numMedical)++;
        }
    }
    // check bottom right
    if(!bottom_edge && !right_edge && RandNumGen(0,10) > threshold){
        int x = i + 1;
        int y = j + 1;
        int directionIndex = (x*Y_MAX) + y;
        int colorIndex = 2 * ((x*Y_MAX) + y) + 1;
        grid->directions[directionIndex] = DOWN_RIGHT;
        cl_float3 color = grid->vectorArray[colorIndex];
        if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z) {
            grid->vectorArray[colorIndex] = YELLOW;
            (*grid->numMedical)++;
        }
    }
}
int main() {
    // WindowManager and Renderer are classes from my Comp 371 Project that I am re-using here for quick startup/clean code.
    // Both of these classes were written/converted by me and I can prove it through GitHub if necessary!
    // Shader.h is borrowed from LearnOpenGL.com
    std::vector<cl::Platform> platforms;
    cl::Device gpu= GetDeviceTarget(CL_DEVICE_TYPE_GPU);
    cl::Device cpu = GetDeviceTarget(CL_DEVICE_TYPE_CPU);
    cl::Context gpu_context = cl::Context(gpu);
    cl::Context cpu_context = cl::Context(cpu);
    grid = new Grid(SCREEN_WIDTH, SCREEN_HEIGHT, gpu_context);
    // set a refer
    float const REFRESH_TIME = 1/30.0f;
    int const SIZE = (Y_MAX*X_MAX*2);
    WindowManager::Initialize("OpenCL Project: Cancer Cell Simulation", SCREEN_WIDTH*POINT_SIZE, SCREEN_HEIGHT*POINT_SIZE);
    Renderer::Initialize(glm::vec3(0,0.5,0.5), POINT_SIZE);
    // add our shaders
    Renderer::addShader(new Shader("../Shaders/vertex_shader.glsl","../Shaders/frag_shader.glsl"));
    Renderer::setRenderMode(GL_POINTS);
    Renderer::useShader(0);

    cl::Program gpu_program = buildProgram(gpu_context, gpu, PROGRAM);
    //create queue to which we will push commands for the device.
    cl::CommandQueue gqueue(gpu_context,gpu);
    cl::CommandQueue cqueue(cpu_context, cpu, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE);
    glm::mat4 projection = glm::ortho(0.0f, SCREEN_WIDTH*POINT_SIZE, SCREEN_HEIGHT*POINT_SIZE, 0.0f, -1.0f, 1.0f);
    Renderer::getCurrentShader()->setMat4("orthoMatrix", projection);



    cl::Kernel mutation=cl::Kernel(gpu_program,"mutation_kernel");
    cl::Kernel expansion=cl::Kernel(gpu_program,"radial_expansion");
    mutation.setArg(0,grid->vectorArray);
    mutation.setArg(1, X_MAX);
    mutation.setArg(2, Y_MAX);
    mutation.setArg(3,grid->numCancer);
    mutation.setArg(4,grid->numMedical);
    expansion.setArg(0, grid->vectorArray);
    expansion.setArg(1, grid->directions);
    expansion.setArg(2, X_MAX);
    expansion.setArg(3, Y_MAX);
    expansion.setArg(4,grid->numMedical);
    expansion.setArg(5,grid->numCancer);
    grid->CreateVAO();
    float timer=0;
   do{
        WindowManager::Update();
        // Update World
        Renderer::BeginFrame();
        // set the color
        if(timer >= REFRESH_TIME) {
            cl::Event events[3];
            cqueue.enqueueNativeKernel(&InjectionKernel, std::make_pair(&grid, sizeof(Grid)));
            cqueue.enqueueNativeKernel(&InjectionKernel, std::make_pair(&grid, sizeof(Grid)));
            cqueue.enqueueNativeKernel(&InjectionKernel, std::make_pair(&grid, sizeof(Grid)));
            cqueue.finish();
            gqueue.enqueueNDRangeKernel(mutation,cl::NullRange,cl::NDRange(X_MAX, Y_MAX),cl::NullRange);
            gqueue.enqueueNDRangeKernel(expansion,cl::NullRange,cl::NDRange(X_MAX, Y_MAX),cl::NullRange);
            gqueue.finish();
            grid->UpdateVAO();
            PrintOutput(grid);
        }else{
            timer += WindowManager::GetFrameTime();
        }
        cout << "Time since last frame: " << WindowManager::GetFrameTime() << endl;
        grid->Draw();
        Renderer::EndFrame();
   }while(!WindowManager::ExitWindow());
    //shutdown the renderer
     Renderer::Shutdown();
     // delete the grid and unallocate SVM data
     delete grid;
}
