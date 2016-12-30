//
// Created by wangyang on 16/11/24.
//

#include "FGWindow.h"
#include <stdio.h>
#include <iostream>

#define WindowWidth 200
#define WindowHeight 100
#define WindowPosX 0
#define WindowPosY 1920 - WindowHeight

FGWindow *fgWindow;


void resize_callback(GLFWwindow *window ,int w,int h) {
    fgWindow->resize(w, h);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        fgWindow->keyPressed(key);
    } else if (action == GLFW_RELEASE) {
        fgWindow->keyReleased(key);
    }
}

void focus_callback(GLFWwindow *window, int focus) {

}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    fgWindow->mouseMove(xpos, ypos);
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        fgWindow->mousePressed(button);
    } else {
        fgWindow->mouseReleased(button);
    }
}

GLFWwindow *initGLFWwindow() {
    /* Initialize the library */
    if (!glfwInit()) {
        printf("GLFW Init Failed!!!");
        return NULL;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 3);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow *glfwWindow = glfwCreateWindow(WindowWidth, WindowHeight, "Hello World", NULL, NULL);
    if (!glfwWindow)
    {
        printf("GLFW Window create failed!!!");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(glfwWindow);

    glfwSetWindowPos(glfwWindow,WindowPosX,WindowPosY);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return NULL;
    }

    printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
    if (GLVersion.major < 2) {
        printf("Your system doesn't support OpenGL >= 2!\n");
        return NULL;
    }

    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
           glGetString(GL_SHADING_LANGUAGE_VERSION));

    glfwSwapInterval(1);
    glfwSetWindowSizeCallback(glfwWindow,resize_callback);
    glfwSetKeyCallback(glfwWindow, key_callback);
    glfwSetWindowFocusCallback(glfwWindow,focus_callback);
    glfwSetMouseButtonCallback(glfwWindow,mouse_callback);
    glfwSetCursorPosCallback(glfwWindow, cursor_position_callback);
    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return glfwWindow;
}

int main(int argc,char **argv) {
    // setup assets

    ELAssets::shared()->addSearchPath("/Users/ocean/Documents/Codes/On Git/EZGLKit/Projects/ClionProj/");
    ELAssets::shared()->addSearchPath("/Users/ocean/Documents/Codes/On Git/EZGLKit/Projects/ClionProj/Shader/");
    ELAssets::shared()->addSearchPath("/Users/ocean/Documents/Codes/On Git/EZGLKit/Projects/ClionProj/FPS Game/Textures/");
    ELAssets::shared()->addSearchPath("/Users/ocean/Documents/Codes/On Git/EZGLKit/Projects/ClionProj/FPSGame/Textures/");
    ELAssets::shared()->addSearchPath("/Users/wangyang/Documents/Projects/On Git/EZGLKit/Projects/ClionProj/");
    ELAssets::shared()->addSearchPath("/Users/wangyang/Documents/Projects/On Git/EZGLKit/Projects/ClionProj/Shader/");
    ELAssets::shared()->addSearchPath("/Users/wangyang/Documents/Projects/On Git/EZGLKit/Projects/ClionProj/FPS Game/Textures/");
    ELAssets::shared()->addSearchPath("/Users/wangyang/Documents/Projects/On Git/EZGLKit/Projects/ClionProj/FPSGame/Textures/");


    GLFWwindow *glfwWindow = initGLFWwindow();
    fgWindow = new FGWindow(glfwWindow, WindowWidth, WindowHeight);
    fgWindow->run();
    return 1;
}