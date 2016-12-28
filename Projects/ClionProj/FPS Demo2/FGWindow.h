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
    FGWindow(GLFWwindow* glfwWindow, int width, int height);
    void run();
    void update();
    void render();

    void keyPressed(int key);
    void keyReleased(int key);
    void mousePressed(int mouseButton);
    void mouseReleased(int mouseButton);
    void mouseMove(int x, int y);
    void resize(int w, int h);
private:
    GLFWwindow* glfwWindow;
    ELWorld *world;
    FGScene *mainScene;
    double lastXPos;
    double lastYPos;
};


#endif //EZGL_FGWINDOW_H
