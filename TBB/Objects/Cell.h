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
#include "tbb/spin_mutex.h"
#define UP 0
#define UP_RIGHT 1
#define UP_LEFT 2
#define DOWN 3
#define DOWN_RIGHT 4
#define DOWN_LEFT 5
#define LEFT 6
#define RIGHT 7

class Cell {
private:
    float *x;
    float *y;
    glm::vec3 *color;
    tbb::spin_mutex *mtx = new tbb::spin_mutex();
    tbb::spin_mutex *direction_lock = new tbb::spin_mutex();
    int *direction = 0;
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
    void setDirection(int);
    int getDirection();
};


#endif //A1_CELL_H
