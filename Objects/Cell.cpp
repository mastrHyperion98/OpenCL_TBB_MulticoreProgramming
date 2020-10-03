//
// Created by steven on 2020-09-22.
//

#include "Cell.h"
Cell::Cell(float _x, float _y, glm::vec3 _color) {
    setPosition(_x, _y);
    setColor(_color);

}
Cell::Cell(const Cell &cell){
    x = new float(*cell.x);
    y = new float(*cell.y);
    color = new glm::vec3(cell.color->x,cell.color->y, cell.color->z);
}

Cell::~Cell() {
    delete x;
    delete y;
    delete color;
    delete mtx;
    delete pos_lock;
}
Cell & Cell::operator=(const Cell &cell){
    if(&cell == this)
        return *this;
    *x = *cell.x;
    *y = *cell.y;
    color->x = cell.color->x;
    color->y = cell.color->y;
    color->z = cell.color->z;

   return *this;
}

void Cell::setColor(glm::vec3 _color) {
    mtx->lock();
    color = new glm::vec3(_color.x, _color.y, _color.z);
    mtx->unlock();
}

glm::vec3 Cell::getColor() {
    mtx->lock();
    glm::vec3 cellColor = glm::vec3(color->x, color->y, color->z);
    mtx->unlock();
    return cellColor;
}

void Cell::setPosition(float _x, float _y) {
    pos_lock->lock();
    x = new float(_x);
    y = new float(_y);
    pos_lock->unlock();
}

float Cell::getX() {
    pos_lock->lock();
    float _x(*x);
    pos_lock->unlock();
    return _x;
}

float Cell::getY() {
    pos_lock->lock();
    float _y(*y);
    pos_lock->unlock();
    return _y;
}

