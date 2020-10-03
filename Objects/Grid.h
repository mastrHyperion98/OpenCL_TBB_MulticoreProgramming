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
using namespace std;


class Grid {
private:
    int numRows;
    int numCols;
    GLuint vao = -1;
    GLuint vbo = -1;
    std::mutex access;

public:
    Cell*** matrix;
    vector<Cell*> cancerList;
    vector<Cell*> medicineList;
    Grid(int, int);
    ~Grid();
    Grid(const Grid &grid);
    Grid &operator=(const Grid &grid);
    void UpdateColor(int,int,glm::vec3);
    void UpdateVAO();
    void CreateVAO();
    void Draw();

};


#endif //A1_GRID_H
