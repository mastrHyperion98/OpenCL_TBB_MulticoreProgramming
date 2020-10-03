//
// Created by steven on 2020-10-02.
//

#include "Grid.h"
#include "random"
#include <set>
Grid::Grid(int x, int y){
    numRows = x;
    numCols = y;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> x_rand(0,numRows-1); // dist
    std::uniform_int_distribution<std::mt19937::result_type> y_rand(0,numCols-1); // dist
    int num_cancer = 0.25f * (numRows * numCols);
    set<tuple<int,int>> set;
    cancerList.reserve(num_cancer);

    while(set.size() <= num_cancer - 1 ){
        int x1 = x_rand(rng);
        int y1 = y_rand(rng);
        set.insert(tuple<int,int>(x1,y1));
    }

    matrix = new Cell**[numRows];
    for(int i = 0; i < numRows; i++)
       matrix[i] = new Cell*[numCols];

    for(int i = 0; i < numRows; i++){
        for(int j= 0; j < numCols; j++){
            glm::vec3 color = GREEN;
            if(set.find(tuple<int,int>(i,j)) != set.end()){
                color = RED;
            }
            matrix[i][j] = new Cell(i,j, color);
            if(color == RED)
                cancerList.push_back(matrix[i][j]);
        }
    }
}

Grid::~Grid() {
    for(long i = 0; i < numRows; i++){
        for(long j= 0; j < numCols; j++){
            delete matrix[i][j];
        }
        delete matrix[i];
    }
    delete matrix;
}

Grid::Grid(const Grid &grid){
    numCols = grid.numCols;
    numRows = grid.numRows;

    matrix = new Cell**[numRows];
    for(int i = 0; i < numRows; i++)
        matrix[i] = new Cell*[numCols];

    for(int i = 0; i < numRows; i++){
        for(int j= 0; j < numCols; j++){
            matrix[i][j] = new Cell(*grid.matrix[i][j]);
        }
    }
    cancerList = grid.cancerList;
    medicineList = grid.medicineList;
    vao = grid.vao;
    vbo = grid.vbo;
}
Grid & Grid::operator=(const Grid &grid){
    numCols = grid.numCols;
    numRows = grid.numRows;

    matrix = new Cell**[numRows];
    for(int i = 0; i < numRows; i++)
        matrix[i] = new Cell*[numCols];

    for(int i = 0; i < numRows; i++){
        for(int j= 0; j < numCols; j++){
            matrix[i][j] = new Cell(*grid.matrix[i][j]);
        }
    }
    cancerList = grid.cancerList;
    medicineList = grid.medicineList;
    vao = grid.vao;
    vbo = grid.vbo;

    return *this;
}

void Grid::CreateVAO(){
    // Create a vertex array
    vector<glm::vec3> positionArray;
    for(int i = 0; i < numRows; i++){
        for(int j= 0; j < numCols; j++){
            Cell *cell = matrix[i][j];
            positionArray.push_back(glm::vec3(cell->getX(), cell->getY(), 0));
            positionArray.push_back(cell->getColor());
        }
    }

    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, positionArray.size() * sizeof(glm::vec3), &positionArray[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
                          3,                   // size
                          GL_FLOAT,            // type
                          GL_FALSE,            // normalized?
                          2*sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
                          (void*)0           // array buffer offset
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,                   // attribute 0 matches aPos in Vertex Shader
                          3,                   // size
                          GL_FLOAT,            // type
                          GL_FALSE,            // normalized?
                          2*sizeof(glm::vec3),
                          (void*)sizeof(glm::vec3)     // array buffer offset
    );
    glEnableVertexAttribArray(1);
    vbo = vertexBufferObject;
    vao = vertexArrayObject;
}

void Grid::UpdateVAO(){
    // Create a vertex array
    access.lock();
    vector<glm::vec3> positionArray;
    for(int i = 0; i < numRows; i++){
        for(int j= 0; j < numCols; j++){
            Cell *cell = matrix[i][j];
            positionArray.push_back(glm::vec3(cell->getX(), cell->getY(), 0));
            positionArray.push_back(cell->getColor());
        }
    }
    access.unlock();

    glBindVertexArray(vao);
    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, positionArray.size() * sizeof(glm::vec3), &positionArray[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
                          3,                   // size
                          GL_FLOAT,            // type
                          GL_FALSE,            // normalized?
                          2*sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
                          (void*)0           // array buffer offset
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,                   // attribute 0 matches aPos in Vertex Shader
                          3,                   // size
                          GL_FLOAT,            // type
                          GL_FALSE,            // normalized?
                          2*sizeof(glm::vec3),
                          (void*)sizeof(glm::vec3)     // array buffer offset
    );
    glEnableVertexAttribArray(1);
}

void Grid::Draw() {
    // load the proper vao
    glBindVertexArray(vao);
    glDrawArrays(Renderer::getRenderMode(), 0, numCols*numRows);
}

void Grid::UpdateColor(int i , int j , glm::vec3 color) {
    access.lock();
    matrix[i][j]->setColor(color);
    access.unlock();
}