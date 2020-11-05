//
// Created by steven on 2020-10-02.
//

#include "Grid.h"
#include <random>
#include <set>
#include <algorithm>

Grid::Grid(int x, int y){
    numRows = x;
    numCols = y;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> x_rand(0,numRows-1); // dist
    std::uniform_int_distribution<std::mt19937::result_type> y_rand(0,numCols-1); // dist
    int num_cancer = 0.50f * (numRows * numCols);
    set<tuple<int,int>> set;
    numCancer = new int(0);
    numMedical=new int(0);

    while(set.size() <= num_cancer - 1 ){
        int x1 = x_rand(rng);
        int y1 = y_rand(rng);
        set.insert(tuple<int,int>(x1,y1));
    }

    vectorArray = new cl_float3[(numRows*numCols*2)];
    directions = new int[numRows*numCols*2];
    for(int i = 0; i < numRows; i++){
        for(int j= 0; j < numCols; j++){
            cl_float3 position = cl_float3{float(i), float(j), 0.0f};
            cl_float3 color = GREEN;
            if(set.find(tuple<int,int>(i,j)) != set.end()){
                color = RED;
            }
            //matrix[i][j] = new Cell(i,j, color);
            int colorIndex = getVaoColorIndex(i,j);
            int positionIndex = colorIndex - 1;
            vectorArray[positionIndex] = position;
            vectorArray[colorIndex] = color;
            // set all directions to -1;
            directions[(i*numCols)+j]= -1;
            if(color.x == RED.x && color.y == RED.y && color.z == RED.z)
                (*numCancer)++;
        }
    }
}

Grid::~Grid() {
    delete vectorArray;
    delete directions;
    delete numCancer;
    delete numMedical;
}

Grid::Grid(const Grid &grid){
    numCols = grid.numCols;
    numRows = grid.numRows;

    vectorArray = new cl_float3[(numRows*numCols*2)];
    for(int i = 0; i < numRows*numCols*2; i++){
           vectorArray[i] = grid.vectorArray[i];
    }
    numMedical = new int(*grid.numMedical);
    numCancer = new int(*grid.numCancer);
    vao = grid.vao;
    vbo = grid.vbo;
}
Grid & Grid::operator=(const Grid &grid){
    numCols = grid.numCols;
    numRows = grid.numRows;

    vectorArray = new cl_float3[(numRows*numCols*2)];
    for(int i = 0; i < numRows*numCols*2; i++){
        vectorArray[i] = grid.vectorArray[i];
    }
    *numMedical = *grid.numMedical;
    *numCancer = *grid.numCancer;
    vao = grid.vao;
    vbo = grid.vbo;

    return *this;
}

void Grid::CreateVAO(){
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, (numRows*numCols*2)* sizeof(cl_float3), &vectorArray[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
                          3,                   // size
                          GL_FLOAT,            // type
                          GL_FALSE,            // normalized?
                          2*sizeof(cl_float3), // stride - each vertex contain 2 vec3 (position, color)
                          (void*)0           // array buffer offset
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,                   // attribute 0 matches aPos in Vertex Shader
                          3,                   // size
                          GL_FLOAT,            // type
                          GL_FALSE,            // normalized?
                          2*sizeof(cl_float3),
                          (void*)sizeof(cl_float3)     // array buffer offset
    );
    glEnableVertexAttribArray(1);
    vbo = vertexBufferObject;
    vao = vertexArrayObject;
}

void Grid::UpdateVAO(){
    // Create a vertex array
    glBindVertexArray(vao);
    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (numRows*numCols*2) * sizeof(cl_float3), &vectorArray[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
                          3,                   // size
                          GL_FLOAT,            // type
                          GL_FALSE,            // normalized?
                          2*sizeof(cl_float3), // stride - each vertex contain 2 vec3 (position, color)
                          (void*)0           // array buffer offset
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,                   // attribute 0 matches aPos in Vertex Shader
                          3,                   // size
                          GL_FLOAT,            // type
                          GL_FALSE,            // normalized?
                          2*sizeof(cl_float3),
                          (void*)sizeof(cl_float3)     // array buffer offset
    );
    glEnableVertexAttribArray(1);
}

void Grid::Draw() {
    // load the proper vao
    glBindVertexArray(vao);
    glDrawArrays(Renderer::getRenderMode(), 0, numCols*numRows);
}

void Grid::UpdateColor(int i , int j , cl_float3 color) {
    int index = getVaoColorIndex(i,j);
    vectorArray[index] = color;
}

void Grid::IncremementMedical() {
    medicalAccess.lock();
    (*numMedical)++;
    medicalAccess.unlock();
}

void Grid::IncrementCancer() {
    cancerAccess.lock();
    (*numCancer)++;
    cancerAccess.unlock();
}

void Grid::AddMedical(int num) {
    medicalAccess.lock();
    (*numMedical) += num;
    medicalAccess.unlock();
}

void Grid::DecrementCancer() {
    cancerAccess.lock();
    (*numCancer)--;
    cancerAccess.unlock();
}

void Grid::DecrementMedical() {
    medicalAccess.lock();
    (*numMedical)--;
    medicalAccess.unlock();
}

void Grid::RemoveFromMedical(int num) {
    medicalAccess.lock();
    numMedical-=num;
    medicalAccess.unlock();
}

int Grid::getNumMedical() {
    int tmp;
    medicalAccess.lock();
    tmp = *numMedical;
    medicalAccess.unlock();
    return tmp;
}

int Grid::getNumCancer() {
    int tmp;
    cancerAccess.lock();
    tmp = *numCancer;
    cancerAccess.unlock();
    return tmp;
}

bool Grid::float3Equal(cl_float3 obj, cl_float3 other) {
    return (obj.x == other.x && obj.y == other.y && obj.z == other.z);
}