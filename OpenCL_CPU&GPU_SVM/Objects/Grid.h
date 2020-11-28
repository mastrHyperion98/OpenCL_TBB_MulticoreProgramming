//
// Created by steven on 2020-10-02.
//

#ifndef A1_GRID_H
#define A1_GRID_H
#define GREEN cl_float3{0.0f,0.8f,102/255.0f}
#define RED cl_float3{1.0f,0.0f,0.0f}
#define YELLOW cl_float3{1.0f,1.0f,0.0f}
#define UP  0
#define UP_RIGHT    1
#define UP_LEFT 2
#define DOWN    3
#define DOWN_RIGHT  4
#define DOWN_LEFT   5
#define LEFT    6
#define RIGHT   7

#include <GL/glew.h>
#include "vector"
#include "glm/glm.hpp"
#include <mutex>
#include "CL/cl2.hpp"


using namespace std;

class Grid {
private:
    int numRows;
    int numCols;
    GLuint vao = -1;
    GLuint vbo = -1;
    cl::Context context;
    cl::SVMAllocator<cl_float3, cl::SVMTraitFine<cl::SVMTraitReadWrite<>>> arrayAllocator;
    cl::SVMAllocator<int, cl::SVMTraitFine<cl::SVMTraitReadWrite<>>> intAllocator;
public:
    cl_float3 * vectorArray;
    int *directions;
    int *numCancer;
    int *numMedical;
    Grid(int, int, cl::Context);
    ~Grid();
    Grid(const Grid &grid);
    Grid &operator=(const Grid &grid);
    void UpdateColor(int,int,cl_float3);
    void UpdateVAO();
    void CreateVAO();
    void Draw();
};


#endif //A1_GRID_H
