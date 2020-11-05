//
// Created by steven on 2020-10-02.
//

#ifndef A1_GRID_H
#define A1_GRID_H
#define GREEN cl_float3{0.0f,0.8f,102/255.0f}
#define RED cl_float3{1.0f,0.0f,0.0f}
#define YELLOW cl_float3{1.0f,1.0f,0.0f}

#include "vector"
#include "Cell.h"
#include "glm/glm.hpp"
#include <mutex>
#include "CL/cl2.hpp"
#include "tbb/concurrent_vector.h"
#define UP 0
#define UP_RIGHT 1
#define UP_LEFT 2
#define DOWN 3
#define DOWN_RIGHT 4
#define DOWN_LEFT 5
#define LEFT 6
#define RIGHT 7

using namespace std;
using namespace tbb;

class Grid {
private:
    int numRows;
    int numCols;
    GLuint vao = -1;
    GLuint vbo = -1;
    std::mutex cancerAccess;
    std::mutex medicalAccess;


public:
    cl_float3* vectorArray;
    int *directions;
    int *numCancer;
    int *numMedical;
    Grid(int, int);
    ~Grid();
    Grid(const Grid &grid);
    Grid &operator=(const Grid &grid);
    void UpdateColor(int,int,cl_float3);
    void UpdateVAO();
    void CreateVAO();
    void Draw();
    void IncrementCancer();
    void DecrementCancer();
    void DecrementMedical();
    void RemoveFromMedical(int num);
    void IncremementMedical();
    void AddMedical(int num);
    int getNumCancer();
    int getNumMedical();
    int getVaoColorIndex(int i, int j){return 2*((i*numCols)+j)+1;}
    bool float3Equal(cl_float3 obj, cl_float3 other);
};


#endif //A1_GRID_H
