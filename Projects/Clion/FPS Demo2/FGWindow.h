//
// Created by wangyang on 16/12/2.
//

#ifndef EZGL_FGWINDOW_H
#define EZGL_FGWINDOW_H

#include "EZGL.h"

class FGScene;
class GLFWwindow;

class FGWindow {
public:
    FGWindow();
    void run();
private:
    GLFWwindow* glfwWindow;
    ELWorld *world;
    FGScene *mainScene;
};


#endif //EZGL_FGWINDOW_H
