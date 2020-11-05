//
// Created by steven on 2020-10-02.
//

#ifndef A1_GRID_H
#define A1_GRID_H
#define GREEN glm::vec3(0.0f,0.8f,102/255.0f)
#define RED glm::vec3(1.0f,0.0f,0.0f)

#include "vector"
#include "Cell.h"
#include "glm/glm.hpp"
#include <mutex>
#include "tbb/concurrent_vector.h"
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
    int numCancer;
    int numMedical;
    concurrent_vector<glm::vec3> positionArray;
public:
    Cell*** matrix;

    Grid(int, int);
    ~Grid();
    Grid(const Grid &grid);
    Grid &operator=(const Grid &grid);
    void UpdateColor(int,int,glm::vec3);
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
};


#endif //A1_GRID_H
