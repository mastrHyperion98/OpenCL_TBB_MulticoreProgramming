//
// Created by steven on 2020-09-22.
//

#include "Cell.h"
Cell::Cell(float _x, float _y, glm::vec3 _color) {
    setPosition(_x, _y);
    setColor(_color);
    direction = new int(-1);
}
Cell::Cell(const Cell &cell){
    x = new float(*cell.x);
    y = new float(*cell.y);
    color = new glm::vec3(cell.color->x,cell.color->y, cell.color->z);
    direction = new int(*cell.direction);
}

Cell::~Cell() {
    delete x;
    delete y;
    delete color;
    delete mtx;
    delete direction_lock;
    delete direction;
}
Cell & Cell::operator=(const Cell &cell){
    if(&cell == this)
        return *this;
    *x = *cell.x;
    *y = *cell.y;
    color->x = cell.color->x;
    color->y = cell.color->y;
    color->z = cell.color->z;
    *direction = *cell.direction;
   return *this;
}

void Cell::setColor(glm::vec3 _color) {
    mtx->lock();
    delete color;
    color = new glm::vec3(_color.x, _color.y, _color.z);
    mtx->unlock();
}

glm::vec3 Cell::getColor() {
    tbb::spin_mutex::scoped_lock(mtx);
    glm::vec3 cellColor = glm::vec3(color->x, color->y, color->z);
    return cellColor;
}

void Cell::setPosition(float _x, float _y) {
    delete x;
    delete y;
    x = new float(_x);
    y = new float(_y);
}

float Cell::getX() {
    float _x(*x);
    return _x;
}

float Cell::getY() {
    float _y(*y);
    return _y;
}

int Cell::getDirection() {
    tbb::spin_mutex::scoped_lock(direction_lock);
    int dir = *direction;
    return dir;
}

void Cell::setDirection(int dir) {
    tbb::spin_mutex::scoped_lock(direction_lock);
    *direction = dir;
}