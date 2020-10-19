#include <iostream>
// Import glew and glfw3 ORDER MATTERS
#include "GL/glew.h"   // Include GLEW - OpenGL Extension Wrangler
#include "GLFW/glfw3.h" // GLFW provides a cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs
// Import glm
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" // include this to create transformation matrices
#include "Framework/WindowManager.h"
#include "Framework/Renderer.h"
#include <thread>
#include "random"
#include "Objects/Grid.h"
#include <deque>
#include "tbb/task.h"

#define SCREEN_WIDTH 1024.0f
#define SCREEN_HEIGHT 768.0f
#define POINT_SIZE 1.0f
#define GREEN glm::vec3(0.0f,0.8f,102/255.0f)
#define RED glm::vec3(1.0f,0.0f,0.0f)
#define YELLOW glm::vec3(1.0f,1.0f,0.0f)

using namespace std;
using namespace glm;
using namespace tbb;

Grid *grid = new Grid(SCREEN_WIDTH, SCREEN_HEIGHT);
int const MUTATION_THRESHOLD = 6; // errors in slide
int const MUTATION_THRESHOLD_CORNER = 3;
int const MUTATION_THRESHOLD_EDGE = 4;
int const X_MAX = SCREEN_WIDTH-1;
int const Y_MAX = SCREEN_HEIGHT-1;

int RandNumGen(int min, int max) {
    if(min < 0){
        min = 0;
    }
    if(max < min){
        max = min;
    }
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(min,max); // dist
    return dist(rng);
}

// Checks if a healthy cell is to be turned into a cancer cell
void HealthyCheck(int i, int j){
    int numberCancer = 0;
    // check if corner
    if ((i == 0 || i == X_MAX) && (j == 0 || j == Y_MAX)) {
        // there are only four corners i == 0 j == 0
        if (i == 0 && j == 0) { // top left corner
            // only three checks can be done
            if (grid->matrix[i][j + 1]->getColor() == RED) // down
                numberCancer++;
            if (grid->matrix[i + 1][j]->getColor() == RED) // right
                numberCancer++;
            if (grid->matrix[i + 1][j + 1]->getColor() == RED) // down-right
                numberCancer++;
        } else if (i == X_MAX && j == 0) { // top right
            if (grid->matrix[i][j + 1]->getColor() == RED) // down
                numberCancer++;
            if (grid->matrix[i - 1][j]->getColor() == RED) // left
                numberCancer++;
            if (grid->matrix[i - 1][j + 1]->getColor() == RED) // left-down
                numberCancer++;
        } else if (j == Y_MAX && i == X_MAX) { // bottom right
            if (grid->matrix[i][j - 1]->getColor() == RED) // up
                numberCancer++;
            if (grid->matrix[i - 1][j]->getColor() == RED) // left
                numberCancer++;
            if (grid->matrix[i - 1][j - 1]->getColor() == RED) // up-left
                numberCancer++;
        } else { // bottom left
            if (grid->matrix[i][j - 1]->getColor() == RED)  // up
                numberCancer++;
            if (grid->matrix[i + 1][j]->getColor() == RED) // right
                numberCancer++;
            if (grid->matrix[i + 1][j - 1]->getColor() == RED) // up-right
                numberCancer++;
        }
        if (numberCancer >= MUTATION_THRESHOLD_CORNER) {
            grid->UpdateColor(i, j, RED);
            grid->IncrementCancer();
        }
    }
        // check if edge
        // is edge if i == 0 or width or if j == 0 or height
    else if (i == 0 || i == X_MAX || j == 0 || j == Y_MAX) {
        // only four options is left, bottom, right or top
        if (j == 0) {
            // top row
            if (grid->matrix[i - 1][j]->getColor() == RED) // left
                numberCancer++;
            if (grid->matrix[i - 1][j + 1]->getColor() == RED) // bottom-left
                numberCancer++;
            if (grid->matrix[i+1][j]->getColor() == RED) // right
                numberCancer++;
            if (grid->matrix[i + 1][j + 1]->getColor() == RED) // bottom right
                numberCancer++;
            if (grid->matrix[i][j+1]->getColor() == RED) // bottom
                numberCancer++;
        } else if (i == X_MAX) { // right side
            if (grid->matrix[i][j-1]->getColor() == RED) // up
                numberCancer++;
            if (grid->matrix[i - 1][j - 1]->getColor() == RED) // up-left
                numberCancer++;
            if (grid->matrix[i-1][j]->getColor() == RED) // left
                numberCancer++;
            if (grid->matrix[i-1][j + 1]->getColor() == RED) // bottom lleft
                numberCancer++;
            if (grid->matrix[i][j + 1]->getColor() == RED) // bottom
                numberCancer++;
        } else if (j == Y_MAX) { // bottom row
            if (grid->matrix[i][j-1]->getColor() == RED) // up
                numberCancer++;
            if (grid->matrix[i - 1][j - 1]->getColor() == RED) // up-left
                numberCancer++;
            if (grid->matrix[i-1][j]->getColor() == RED) // left
                numberCancer++;
            if (grid->matrix[i + 1][j - 1]->getColor() == RED) // up right
                numberCancer++;
            if (grid->matrix[i + 1][j]->getColor() == RED) // right
                numberCancer++;
        } else { // left side
            if (grid->matrix[i][j+1]->getColor() == RED) // bottom
                numberCancer++;
            if (grid->matrix[i + 1][j + 1]->getColor() == RED) // bottom-right
                numberCancer++;
            if (grid->matrix[i+1][j]->getColor() == RED) // right
                numberCancer++;
            if (grid->matrix[i][j - 1]->getColor() == RED) // U{P
                numberCancer++;
            if (grid->matrix[i + 1][j - 1]->getColor() == RED) // UP_RIGHT
                numberCancer++;
        }

        if (numberCancer >= MUTATION_THRESHOLD_EDGE) {
            grid->UpdateColor(i, j, RED);
            grid->IncrementCancer();
        }
    } else {
        // else regular threshold
        // all other regular stuff -- here
        // check up
        if (grid->matrix[i][j-1]->getColor() == RED) // bottom
            numberCancer++;
        // check top-left
        if (grid->matrix[i - 1][j - 1]->getColor() == RED) // bottom
            numberCancer++;
        // check top-right
        if (grid->matrix[i + 1][j - 1]->getColor() == RED) // bottom
            numberCancer++;
        // check left
        if (grid->matrix[i-1][j]->getColor() == RED) // bottom
            numberCancer++;
        // check right
        if (grid->matrix[i+1][j]->getColor() == RED) // bottom
            numberCancer++;
        // check bottom-left
        if (grid->matrix[i - 1][j + 1]->getColor() == RED) // bottom
            numberCancer++;
        // check bottom
        if (grid->matrix[i][j+1]->getColor() == RED) // bottom
            numberCancer++;
        // check bottom-right
        if (grid->matrix[i + 1][j + 1]->getColor() == RED) // bottom
            numberCancer++;


        if (numberCancer >= MUTATION_THRESHOLD) {
            grid->UpdateColor(i, j, RED);
            grid->IncrementCancer();
        }

    }
}
// check if a cancer cell is to be turned into a healthy cell/
void CancerCheck(int i, int j){
    int numberMedical = 0;
    vector<Cell *> list_medical;
    // check if corner
    if ((i == 0 || i == X_MAX) && (j == 0 || j == Y_MAX)) {
        // there are only four corners i == 0 j == 0
        if (i == 0 && j == 0) { // top left corner
            // only three checks can be done
            if (grid->matrix[i][j + 1]->getColor() == YELLOW){// down
                numberMedical++;
                list_medical.push_back(grid->matrix[i][j + 1]);
            }
            if (grid->matrix[i + 1][j]->getColor() == YELLOW){// right
                numberMedical++;
                list_medical.push_back(grid->matrix[i+1][j]);
            }
            if (grid->matrix[i + 1][j + 1]->getColor() == YELLOW){// down-right
                numberMedical++;
                list_medical.push_back(grid->matrix[i + 1][j + 1]);
            }
        } else if (i == X_MAX && j == 0) { // top right
            if (grid->matrix[i][j + 1]->getColor() == YELLOW){// down
                numberMedical++;
                list_medical.push_back(grid->matrix[i][j + 1]);
            }
            if (grid->matrix[i - 1][j]->getColor() == YELLOW){// left
                numberMedical++;
                list_medical.push_back(grid->matrix[i - 1][j]);
            }
            if (grid->matrix[i - 1][j + 1]->getColor() == YELLOW){// left-down
                numberMedical++;
                list_medical.push_back(grid->matrix[i - 1][j + 1]);
            }
        } else if (j == Y_MAX && i == X_MAX) { // bottom right
            if (grid->matrix[i][j - 1]->getColor() == YELLOW){// up
                numberMedical++;
                list_medical.push_back(grid->matrix[i][j - 1]);
            }
            if (grid->matrix[i - 1][j]->getColor() == YELLOW){// left
                numberMedical++;
                list_medical.push_back(grid->matrix[i - 1][j]);
            }
            if (grid->matrix[i - 1][j - 1]->getColor() == YELLOW){// up-left
                numberMedical++;
                list_medical.push_back(grid->matrix[i - 1][j - 1]);
            }
        } else { // bottom left
            if (grid->matrix[i][j - 1]->getColor() == YELLOW){ // up
                numberMedical++;
                list_medical.push_back(grid->matrix[i][j - 1]);
            }
            if (grid->matrix[i + 1][j]->getColor() == YELLOW){// right
                numberMedical++;
                list_medical.push_back(grid->matrix[i + 1][j]);
            }
            if (grid->matrix[i + 1][j - 1]->getColor() == YELLOW){// up-right
                numberMedical++;
                list_medical.push_back(grid->matrix[i + 1][j - 1]);
            }
        }
        if (numberMedical >= MUTATION_THRESHOLD_CORNER) {
            grid->UpdateColor(i, j, GREEN);
            grid->DecrementCancer();

            for(auto it=list_medical.begin(); it != list_medical.end(); it++){
                int x = (*it)->getX();
                int y = (*it)->getY();
                // return color to green for all consumed medical cells
                grid->UpdateColor(x,y, GREEN);
                grid->DecrementMedical();
            }

        }
    }
        // check if edge
        // is edge if i == 0 or width or if j == 0 or height
    else if (i == 0 || i == X_MAX || j == 0 || j == Y_MAX) {
        // only four options is left, bottom, right or top
        if (j == 0) {
            // top row
            if (grid->matrix[i - 1][j]->getColor() == YELLOW){// left
                numberMedical++;
                list_medical.push_back(grid->matrix[i - 1][j]);
            }
            if (grid->matrix[i - 1][j + 1]->getColor() == YELLOW){// bottom-left
                numberMedical++;
                list_medical.push_back(grid->matrix[i - 1][j + 1]);
            }
            if (grid->matrix[i+1][j]->getColor() == YELLOW){// right
                numberMedical++;
                list_medical.push_back(grid->matrix[i+1][j]);
            }
            if (grid->matrix[i + 1][j + 1]->getColor() == YELLOW){// bottom right
                numberMedical++;
                list_medical.push_back(grid->matrix[i + 1][j + 1]);
            }
            if (grid->matrix[i][j+1]->getColor() == YELLOW){// bottom
                numberMedical++;
                list_medical.push_back(grid->matrix[i][j+1]);
            }
        } else if (i == X_MAX) { // right side
            if (grid->matrix[i][j-1]->getColor() == YELLOW){// up
                numberMedical++;
                list_medical.push_back(grid->matrix[i][j-1]);
            }
            if (grid->matrix[i - 1][j - 1]->getColor() == YELLOW){// up-left
                numberMedical++;
                list_medical.push_back(grid->matrix[i - 1][j - 1]);
            }
            if (grid->matrix[i-1][j]->getColor() == YELLOW){// left
                numberMedical++;
                list_medical.push_back(grid->matrix[i-1][j]);
            }
            if (grid->matrix[i-1][j + 1]->getColor() == YELLOW){// bottom lleft
                numberMedical++;
                list_medical.push_back(grid->matrix[i-1][j + 1]);
            }
            if (grid->matrix[i][j + 1]->getColor() == YELLOW){// bottom
                numberMedical++;
                list_medical.push_back(grid->matrix[i][j + 1]);
            }
        } else if (j == Y_MAX) { // bottom row
            if (grid->matrix[i][j-1]->getColor() == YELLOW){// up
                numberMedical++;
                list_medical.push_back(grid->matrix[i][j-1]);
            }
            if (grid->matrix[i - 1][j - 1]->getColor() == YELLOW){// up-left
                numberMedical++;
                list_medical.push_back(grid->matrix[i - 1][j - 1]);
            }
            if (grid->matrix[i-1][j]->getColor() == YELLOW){// left
                numberMedical++;
                list_medical.push_back(grid->matrix[i-1][j]);
            }
            if (grid->matrix[i + 1][j - 1]->getColor() == YELLOW){// up right
                numberMedical++;
                list_medical.push_back(grid->matrix[i + 1][j - 1]);
            }
            if (grid->matrix[i + 1][j]->getColor() == YELLOW){// right
                numberMedical++;
                list_medical.push_back(grid->matrix[i + 1][j]);
            }
        } else { // left side
            if (grid->matrix[i][j+1]->getColor() == YELLOW){// bottom
                numberMedical++;
                list_medical.push_back(grid->matrix[i][j+1]);
            }
            if (grid->matrix[i + 1][j + 1]->getColor() == YELLOW){// bottom-right
                numberMedical++;
                list_medical.push_back(grid->matrix[i + 1][j + 1]);
            }
            if (grid->matrix[i+1][j]->getColor() == YELLOW){// right
                numberMedical++;
                list_medical.push_back(grid->matrix[i+1][j]);
            }
            if (grid->matrix[i][j - 1]->getColor() == YELLOW){// U{P
                numberMedical++;
                list_medical.push_back(grid->matrix[i][j - 1]);
            }
            if (grid->matrix[i + 1][j - 1]->getColor() == YELLOW){// UP_RIGHT
                numberMedical++;
                list_medical.push_back(grid->matrix[i + 1][j - 1]);
            }
        }

        if (numberMedical >= MUTATION_THRESHOLD_EDGE) {
            grid->UpdateColor(i, j, GREEN);
            grid->DecrementCancer();

            for(auto & it : list_medical){
                int x = it->getX();
                int y = it->getY();
                // return color to green for all consumed medical cells
                grid->UpdateColor(x,y, GREEN);
                grid->DecrementMedical();
            }
        }
    } else {
        // else regular threshold
        // all other regular stuff -- here
        // check up
        if (grid->matrix[i][j-1]->getColor() == YELLOW){// bottom
            numberMedical++;
            list_medical.push_back(grid->matrix[i][j-1]);
        }
        // check top-left
        if (grid->matrix[i - 1][j - 1]->getColor() == YELLOW){// bottom
            numberMedical++;
            list_medical.push_back(grid->matrix[i - 1][j - 1]);
        }
        // check top-right
        if (grid->matrix[i + 1][j - 1]->getColor() == YELLOW){// bottom
            numberMedical++;
            list_medical.push_back(grid->matrix[i + 1][j - 1]);
        }
        // check left
        if (grid->matrix[i-1][j]->getColor() == YELLOW){// bottom
            numberMedical++;
            list_medical.push_back(grid->matrix[i-1][j]);
        }
        // check right
        if (grid->matrix[i+1][j]->getColor() == YELLOW){// bottom
            numberMedical++;
            list_medical.push_back(grid->matrix[i+1][j]);
        }
        // check bottom-left
        if (grid->matrix[i - 1][j + 1]->getColor() == YELLOW){// bottom
            numberMedical++;
            list_medical.push_back(grid->matrix[i - 1][j + 1]);
        }
        // check bottom
        if (grid->matrix[i][j+1]->getColor() == YELLOW){// bottom
            numberMedical++;
            list_medical.push_back(grid->matrix[i][j+1]);
        }
        // check bottom-right
        if (grid->matrix[i + 1][j + 1]->getColor() == YELLOW){// bottom
            numberMedical++;
            list_medical.push_back(grid->matrix[i + 1][j + 1]);
        }

        if (numberMedical >= MUTATION_THRESHOLD) {
            grid->UpdateColor(i, j, YELLOW);
            grid->DecrementCancer();

            for(auto & it : list_medical){
                int x = it->getX();
                int y = it->getY();
                // return color to green for all consumed medical cells
                grid->UpdateColor(x,y, GREEN);
                grid->DecrementMedical();
            }
        }

    }
}

void Injection(){
    // select a point of injection
    int i =  RandNumGen(0, X_MAX);
    int j = RandNumGen(0, Y_MAX);
    int threshold = RandNumGen(0,9);
    // x and y is our point of injection
    // determine if x and y is in a corner, edge or normal
    // based on position randomly decide whether or not a cell becomes  an medicine cell or not
    /**
     * This algorithm is going to be very similar to the ones for CancerCheck and HealthyCheck were we look at the neighbouring
     * cells. However, a random number is generated to determine the threshold for eligibility. At least one cell will be
     * converted into a medicine cell. However, more than one will be left up to the programs random properties.
     */

    int numberMedical = 0;
    vector<Cell *> list_medical;
    // check if corner
    if ((i == 0 || i == X_MAX) && (j == 0 || j == Y_MAX)) {
        // there are only four corners i == 0 j == 0
        if (i == 0 && j == 0) { // top left corner
            // only three checks can be done
            if (RandNumGen(0,10) > threshold){// down
                numberMedical++;
                list_medical.push_back(grid->matrix[i][j + 1]);
                grid->matrix[i][j+1]->setDirection(DOWN);
            }
            if (RandNumGen(0,10) > threshold){// right
                numberMedical++;
                list_medical.push_back(grid->matrix[i+1][j]);
                grid->matrix[i+1][j]->setDirection(RIGHT);
            }
            if (RandNumGen(0,10) > threshold || numberMedical == 0){// down-right
                list_medical.push_back(grid->matrix[i + 1][j + 1]);
                grid->matrix[i + 1][j + 1]->setDirection(DOWN_RIGHT);
            }
        } else if (i == X_MAX && j == 0) { // top right
            if (RandNumGen(0,10) > threshold){// down
                numberMedical++;
                list_medical.push_back(grid->matrix[i][j + 1]);
                grid->matrix[i][j + 1]->setDirection(DOWN);
            }
            if (RandNumGen(0,10) > threshold){// left
                numberMedical++;
                list_medical.push_back(grid->matrix[i - 1][j]);
                grid->matrix[i - 1][j]->setDirection(LEFT);
            }
            if (RandNumGen(0,10) > threshold || numberMedical==0){// left-down
                list_medical.push_back(grid->matrix[i - 1][j + 1]);
                grid->matrix[i - 1][j + 1]->setDirection(DOWN_LEFT);
            }
        } else if (j == Y_MAX && i == X_MAX) { // bottom right
            if (RandNumGen(0,10) > threshold){// up
                numberMedical++;
                list_medical.push_back(grid->matrix[i][j - 1]);
                grid->matrix[i][j - 1]->setDirection(UP);
            }
            if (RandNumGen(0,10) > threshold){// left
                numberMedical++;
                list_medical.push_back(grid->matrix[i - 1][j]);
                grid->matrix[i - 1][j]->setDirection(LEFT);
            }
            if (RandNumGen(0,10) > threshold || numberMedical == 0){// up-left
                list_medical.push_back(grid->matrix[i - 1][j - 1]);
                grid->matrix[i - 1][j - 1]->setDirection(UP_LEFT);
            }
        } else { // bottom left
            if (RandNumGen(0,10) > threshold){ // up
                numberMedical++;
                list_medical.push_back(grid->matrix[i][j - 1]);
                grid->matrix[i][j - 1]->setDirection(UP);
            }
            if (RandNumGen(0,10) > threshold){// right
                numberMedical++;
                list_medical.push_back(grid->matrix[i + 1][j]);
                grid->matrix[i + 1][j]->setDirection(RIGHT);
            }
            if (RandNumGen(0,10) > threshold || numberMedical == 0){// up-right
                list_medical.push_back(grid->matrix[i + 1][j - 1]);
                grid->matrix[i + 1][j - 1]->setDirection(UP_RIGHT);
            }
        }
    }
        // check if edge
        // is edge if i == 0 or width or if j == 0 or height
    else if (i == 0 || i == X_MAX || j == 0 || j == Y_MAX) {
        // only four options is left, bottom, right or top
        if (j == 0) {
            // top row
            if (RandNumGen(0,10) > threshold){// left
                numberMedical++;
                list_medical.push_back(grid->matrix[i - 1][j]);
                grid->matrix[i - 1][j]->setDirection(LEFT);
            }
            if (RandNumGen(0,10) > threshold){// bottom-left
                numberMedical++;
                list_medical.push_back(grid->matrix[i - 1][j + 1]);
                grid->matrix[i - 1][j + 1]->setDirection(DOWN_LEFT);
            }
            if (RandNumGen(0,10) > threshold){// right
                numberMedical++;
                list_medical.push_back(grid->matrix[i+1][j]);
                grid->matrix[i+1][j]->setDirection(RIGHT);
            }
            if (RandNumGen(0,10) > threshold){// bottom right
                numberMedical++;
                list_medical.push_back(grid->matrix[i + 1][j + 1]);
                grid->matrix[i + 1][j + 1]->setDirection(DOWN_RIGHT);
            }
            if (RandNumGen(0,10) > threshold || numberMedical == 0){// bottom
                list_medical.push_back(grid->matrix[i][j+1]);
                grid->matrix[i][j+1]->setDirection(DOWN);
            }
        } else if (i == X_MAX) { // right side
            if (RandNumGen(0,10) > threshold){// up
                numberMedical++;
                list_medical.push_back(grid->matrix[i][j-1]);
                grid->matrix[i][j-1]->setDirection(UP);
            }
            if (RandNumGen(0,10) > threshold){// up-left
                numberMedical++;
                list_medical.push_back(grid->matrix[i - 1][j - 1]);
                grid->matrix[i - 1][j - 1]->setDirection(UP_LEFT);

            }
            if (RandNumGen(0,10) > threshold){// left
                numberMedical++;
                list_medical.push_back(grid->matrix[i-1][j]);
                grid->matrix[i-1][j]->setDirection(LEFT);
            }
            if (RandNumGen(0,10) > threshold){// bottom lleft
                numberMedical++;
                list_medical.push_back(grid->matrix[i-1][j + 1]);
                grid->matrix[i-1][j + 1]->setDirection(DOWN_LEFT);
            }
            if (RandNumGen(0,10) > threshold || numberMedical){// bottom
                numberMedical++;
                list_medical.push_back(grid->matrix[i][j + 1]);
                grid->matrix[i][j + 1]->setDirection(DOWN);
            }
        } else if (j == Y_MAX) { // bottom row
            if (RandNumGen(0,10) > threshold){// up
                numberMedical++;
                list_medical.push_back(grid->matrix[i][j-1]);
                grid->matrix[i][j-1]->setDirection(UP);
            }
            if (RandNumGen(0,10) > threshold){// up-left
                numberMedical++;
                list_medical.push_back(grid->matrix[i - 1][j - 1]);
                grid->matrix[i - 1][j - 1]->setDirection(UP_LEFT);
            }
            if (RandNumGen(0,10) > threshold){// left
                numberMedical++;
                list_medical.push_back(grid->matrix[i-1][j]);
                grid->matrix[i-1][j]->setDirection(LEFT);
            }
            if (RandNumGen(0,10) > threshold){// up right
                numberMedical++;
                list_medical.push_back(grid->matrix[i + 1][j - 1]);
                grid->matrix[i + 1][j - 1]->setDirection(UP_RIGHT);
            }
            if (RandNumGen(0,10) > threshold || numberMedical==0){// right
                numberMedical++;
                list_medical.push_back(grid->matrix[i + 1][j]);
                grid->matrix[i + 1][j]->setDirection(RIGHT);
            }
        } else { // left side
            if (RandNumGen(0,10) > threshold){// bottom
                numberMedical++;
                list_medical.push_back(grid->matrix[i][j+1]);
                grid->matrix[i][j+1]->setDirection(DOWN);
            }
            if (RandNumGen(0,10) > threshold){// bottom-right
                numberMedical++;
                list_medical.push_back(grid->matrix[i + 1][j + 1]);
                grid->matrix[i + 1][j + 1]->setDirection(DOWN_RIGHT);
            }
            if (RandNumGen(0,10) > threshold){// right
                numberMedical++;
                list_medical.push_back(grid->matrix[i+1][j]);
                grid->matrix[i+1][j]->setDirection(RIGHT);
            }
            if (RandNumGen(0,10) > threshold){// U{P
                numberMedical++;
                list_medical.push_back(grid->matrix[i][j - 1]);
                grid->matrix[i][j - 1]->setDirection(UP);
            }
            if (RandNumGen(0,10) > threshold || numberMedical==0){// UP_RIGHT
                list_medical.push_back(grid->matrix[i + 1][j - 1]);
                grid->matrix[i + 1][j - 1]->setDirection(UP_RIGHT);
            }
        }
    } else {
        // else regular threshold
        // all other regular stuff -- here
        // check up
        if (RandNumGen(0,10) > threshold){// bottom
            numberMedical++;
            list_medical.push_back(grid->matrix[i][j-1]);
            grid->matrix[i][j-1]->setDirection(UP);
        }
        // check top-left
        if (RandNumGen(0,10) > threshold){// bottom
            numberMedical++;
            list_medical.push_back(grid->matrix[i - 1][j - 1]);
            grid->matrix[i - 1][j - 1]->setDirection(UP_LEFT);
        }
        // check top-right
        if (RandNumGen(0,10) > threshold){// bottom
            numberMedical++;
            list_medical.push_back(grid->matrix[i + 1][j - 1]);
            grid->matrix[i + 1][j - 1]->setDirection(UP_RIGHT);
        }
        // check left
        if (RandNumGen(0,10) > threshold){// bottom
            numberMedical++;
            list_medical.push_back(grid->matrix[i-1][j]);
            grid->matrix[i-1][j]->setDirection(LEFT);
        }
        // check right
        if (RandNumGen(0,10) > threshold){// bottom
            numberMedical++;
            list_medical.push_back(grid->matrix[i+1][j]);
            grid->matrix[i+1][j]->setDirection(RIGHT);
        }
        // check bottom-left
        if (RandNumGen(0,10) > threshold){// bottom
            numberMedical++;
            list_medical.push_back(grid->matrix[i - 1][j + 1]);
            grid->matrix[i - 1][j + 1]->setDirection(DOWN_LEFT);
        }
        // check bottom
        if (RandNumGen(0,10) > threshold){// bottom
            numberMedical++;
            list_medical.push_back(grid->matrix[i][j+1]);
            grid->matrix[i][j+1]->setDirection(DOWN);
        }
        // check bottom-right
        if (RandNumGen(0,10) > threshold || numberMedical == 0) {// bottom
            list_medical.push_back(grid->matrix[i + 1][j + 1]);
            grid->matrix[i + 1][j + 1]->setDirection(DOWN_RIGHT);
        }
    }

    for(auto & it : list_medical){
        if((*it).getColor() == GREEN ){
            //if((*it).getColor() == RED)
              //  grid->DecrementCancer();
            int x = it->getX();
            int y = it->getY();
            // return color to green for all consumed medical cells
            grid->UpdateColor(x, y, YELLOW);
            grid->IncremementMedical();
        }
    }
}




void MedicineRadialExpansion(int x, int y, int dir){
    if(dir == UP){
        if(y > 0){
            if(grid->matrix[x][y-1]->getColor() == RED)
                grid->DecrementCancer();
            grid->UpdateColor(x, y-1, YELLOW);
            grid->matrix[x][y-1]->setDirection(UP);
        }else {
            grid->DecrementMedical();
        }
        grid->UpdateColor(x,y, GREEN);
    }
    else if(dir == UP_RIGHT){
        if(x < X_MAX && y > 0){
            if(grid->matrix[x+1][y-1]->getColor() == RED)
                grid->DecrementCancer();
            grid->UpdateColor(x+1, y-1, YELLOW);
            grid->matrix[x+1][y-1]->setDirection(UP_RIGHT);
        }else {
            grid->DecrementMedical();
        }
        grid->UpdateColor(x,y, GREEN);
    }
    else if(dir == UP_LEFT){
        if(x > 0 && y > 0){
            if(grid->matrix[x-1][y-1]->getColor() == RED)
                grid->DecrementCancer();
            grid->UpdateColor(x-1, y-1, YELLOW);
            grid->matrix[x-1][y-1]->setDirection(UP_LEFT);
        }else {
            grid->DecrementMedical();
        }
        grid->UpdateColor(x,y, GREEN);
    }
    else if(dir == RIGHT){
        if(x < X_MAX){
            if(grid->matrix[x+1][y]->getColor() == RED)
                grid->DecrementCancer();
            grid->UpdateColor(x+1, y, YELLOW);
            grid->matrix[x+1][y]->setDirection(RIGHT);
        }else {
            grid->DecrementMedical();
        }
        grid->UpdateColor(x,y, GREEN);
    }
    else if(dir == LEFT){
        if(x > 0){
            if(grid->matrix[x-1][y]->getColor() == RED)
                grid->DecrementCancer();
            grid->UpdateColor(x-1, y, YELLOW);
            grid->matrix[x-1][y]->setDirection(LEFT);
        }else {
            grid->DecrementMedical();
        }
        grid->UpdateColor(x,y, GREEN);
    }
    else if(dir == DOWN){
        if(y < Y_MAX){
            if(grid->matrix[x][y+1]->getColor() == RED)
                grid->DecrementCancer();
            grid->UpdateColor(x, y+1, YELLOW);
            grid->matrix[x][y+1]->setDirection(DOWN);
        }else {
            grid->DecrementMedical();
        }
        grid->UpdateColor(x,y, GREEN);
    }
    else if(dir == DOWN_RIGHT){
        if(x < X_MAX && y < Y_MAX){
            if(grid->matrix[x+1][y+1]->getColor() == RED)
                grid->DecrementCancer();
            grid->UpdateColor(x+1, y+1, YELLOW);
            grid->matrix[x+1][y+1]->setDirection(DOWN_RIGHT);
        }else {
            grid->DecrementMedical();
        }
        grid->UpdateColor(x,y, GREEN);
    }
    else if(dir == DOWN_LEFT){
        if(x >0 && y < Y_MAX){
            if(grid->matrix[x-1][y+1]->getColor() == RED)
                grid->DecrementCancer();
            grid->UpdateColor(x-1, y+1, YELLOW);
            grid->matrix[x-1][y+1]->setDirection(DOWN_LEFT);
        }else {
            grid->DecrementMedical();
        }
        grid->UpdateColor(x,y, GREEN);
    }
}

class MutationCheckTask: public task{
public:
    int start;
    int end;
    MutationCheckTask(int start_, int end_){
        start=start_;
        end = end_;
    }
    MutationCheckTask(){
        start=0;
        end = 0;
    }
    task* execute(){
        deque<Cell *> medCells;
        // traverse every cell one by one and check for mutation....if color is red skip.
        for(int i = start; i <= end; i++){
            for(int j = 0; j <= Y_MAX; j++){
                if(grid->matrix[i][j]->getColor() == RED){
                    CancerCheck(i,j);
                }
                else if(grid->matrix[i][j]->getColor() == GREEN){
                    HealthyCheck(i,j);
                }
                else{
                    medCells.push_back(grid->matrix[i][j]);
                }
            }
        }
        while(!medCells.empty()){
            Cell * cell = medCells.front();
            MedicineRadialExpansion(cell->getX(), cell->getY(), cell->getDirection());
            medCells.pop_front();
        }
    }
};

class CellInjectionTask: public task{
public:
    CellInjectionTask(){
    }
    task * execute(){
        Injection();
    }
};

class CellUpdate:public task{
public:
    CellUpdate(){

    }
    task * execute(){
        set_ref_count(3);
        CellInjectionTask & task = *new(allocate_child())CellInjectionTask();
        CellInjectionTask & task2 = *new(allocate_child())CellInjectionTask();
        spawn(task);
        spawn(task2);
        wait_for_all();

        const int processor_count =std::thread::hardware_concurrency(); // processor count - 2
        int indices_per_thread = (X_MAX)/processor_count;
        int remaiinder = X_MAX - (indices_per_thread*processor_count);
        set_ref_count(processor_count+1);
        for (int i = 0; i < processor_count; i++) {
            if(i == processor_count - 1){
                MutationCheckTask & task = *new(allocate_child())MutationCheckTask( i * indices_per_thread, ((i + 1) * indices_per_thread) + remaiinder);
                spawn(task);
            }else {
                MutationCheckTask & task = *new(allocate_child())MutationCheckTask(i * indices_per_thread, ((i + 1) * indices_per_thread));
                spawn(task);
            }
        }
        wait_for_all();
    }
};
int main() {
    // WindowManager and Renderer are classes from my Comp 371 Project that I am re-using here for quick startup/clean code.
    // Both of these classes were written/converted by me and I can prove it through GitHub if necessary!
    // Shader.h is borrowed from LearnOpenGL.com
    float const REFRESH_TIME = 1/30.0f;
    WindowManager::Initialize("Multicore Assignment1", SCREEN_WIDTH*POINT_SIZE, SCREEN_HEIGHT*POINT_SIZE);
    Renderer::Initialize(vec3(0,0.5,0.5), POINT_SIZE);
    // add our shaders
    Renderer::addShader(new Shader("../Shaders/vertex_shader.glsl","../Shaders/frag_shader.glsl"));
    Renderer::setRenderMode(GL_POINTS);
    Renderer::useShader(0);

    glm::mat4 projection = glm::ortho(0.0f, SCREEN_WIDTH*POINT_SIZE, SCREEN_HEIGHT*POINT_SIZE, 0.0f, -1.0f, 1.0f);
    Renderer::getCurrentShader()->setMat4("orthoMatrix", projection);
    grid->CreateVAO();

    float timer=0;
   do{
        WindowManager::Update();
        // Update World
        float dt = WindowManager::GetFrameTime();
        Renderer::BeginFrame();
        // set the color
        if(timer >= REFRESH_TIME) {

            long numCancer = grid->getNumCancer();
            long numMed = grid->getNumMedical();
            long numHealthy = (SCREEN_WIDTH * SCREEN_HEIGHT) - numCancer - numMed;
            cout << "Healthy: " << numHealthy << "\nCancer: " << numCancer << "\nMedicine: " << numMed << "\n\n"
                 << endl;
            CellUpdate & task = *new(task::allocate_root())CellUpdate();
            task::spawn_root_and_wait(task);
            grid->UpdateVAO();
            timer = 0;
        }else{
            timer += WindowManager::GetFrameTime();
        }
        cout << WindowManager::GetFrameTime() << endl;
        grid->Draw();
        Renderer::EndFrame();
   }while(!WindowManager::ExitWindow());
    //shutdown the renderer
     Renderer::Shutdown();
     delete grid;
}
