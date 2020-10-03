//
// Created by steven on 2020-09-22.
//

#ifndef A1_CELL_H
#define A1_CELL_H
#include <tuple>
#include "glm/glm.hpp"
#include "GL/glew.h"   // Include GLEW - OpenGL Extension Wrangler
#include "../Framework/Renderer.h"
#include <mutex>
class Cell {
private:
    float *x;
    float *y;
    glm::vec3 *color;
    std::mutex *mtx = new std::mutex();
    std::mutex *pos_lock= new std::mutex();;
public:
    Cell(float _x, float _y, glm::vec3 _color);
    ~Cell();
    Cell(const Cell &cell);
    Cell &operator=(const Cell &cell);
    void setPosition( float _x, float _y);
    void setColor(glm::vec3 _color);
    glm::vec3 getColor();
    float getX();
    float getY();
};


#endif //A1_CELL_H
