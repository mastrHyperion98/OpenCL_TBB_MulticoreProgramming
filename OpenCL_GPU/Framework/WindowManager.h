//
// Created by Steven Smith on 2020-04-04.
// A class created to help and simplify the main driver into something more simple
// and easier to manage.
//


#ifndef INC_371PROCEDURALFOREST_WINDOWMANAGER_H
#define INC_371PROCEDURALFOREST_WINDOWMANAGER_H


#include "GLFW/glfw3.h"
#include <string>
class WindowManager {
public:
    static void Initialize(std::string, int, int);
    static void Shutdown();
    static void Update();
    static  void EnableMouseCursor();
    static void DisableMouseCursor();
    static float GetMouseMotionX();
    static float GetMouseMotionY();
    static GLFWwindow* getWindow();
    static float GetFrameTime();
    static bool ExitWindow();
private:
    static double sLastFrameTime;
    static float sFrameTime;
    static double sLastMousePositionX;
    static float  sMouseDeltaX;
    static  double sLastMousePositionY;
    static  float  sMouseDeltaY;
    static GLFWwindow* window;
};



#endif //INC_371PROCEDURALFOREST_WINDOWMANAGER_H
