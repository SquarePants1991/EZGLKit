//
// Created by wangyang on 16/11/24.
//

#include "EZGL.h"

void init();
void render();
void resize(int w,int h);
void setupWindow(int argc,char **argv);

ELWorld *world;
double lastTime = 0;
const float WindowWidth = 800;
const float WindowHeight = 600;

#include <iostream>
#include "EZGL.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#define VERSION_MAJOR @VERSION_MAJOR@
#define VERSION_MINOR @VERSION_MINOR@

using namespace std;

int main(int argc,char **argv) {

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WindowWidth, WindowHeight, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
    if (GLVersion.major < 2) {
        printf("Your system doesn't support OpenGL >= 2!\n");
        return -1;
    }

    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
           glGetString(GL_SHADING_LANGUAGE_VERSION));

    glfwSwapInterval(1);

    init();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glad_glClearColor(1.0, 1.0, 0.0, 1.0);
        glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 1;
}

void init() {
    // init game world
    world = new ELWorld(WindowWidth / WindowHeight);

    std::string vertexShader = ELFileUtil::stringContentOfFile("/Users/wangyang/Documents/Projects/On Git/EZGLKit/Projects/Clion/Shader/vertex.glsl");
    std::string fragShader = ELFileUtil::stringContentOfFile("/Users/wangyang/Documents/Projects/On Git/EZGLKit/Projects/Clion/Shader/frag.glsl");
    ELEffect *effect = new ELEffect(vertexShader.c_str(), fragShader.c_str());

    ELLight *defaultLight = new ELLight();
    defaultLight->position = ELVector3Make(2,2,2);
    defaultLight->color = ELVector4Make(1.0,1.0,1.0,1.0);
    defaultLight->intensity = 1.0;
    world->addNode(defaultLight);

    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0, 1, 0);
    ELCubeGeometry *cube = new ELCubeGeometry(ELVector3Make(1,1,1));
    gameObject->addComponent(cube);
    gameObject->addComponent(effect);

    ELGameObject *gameObject2 = new ELGameObject(world);
    world->addNode(gameObject2);
    gameObject2->transform->position = ELVector3Make(0, 0, 0);
    ELPlaneGeometry *plane = new ELPlaneGeometry(ELVector2Make(100,100));
    gameObject2->addComponent(plane);
    gameObject2->addComponent(effect);

    glEnable(GL_CULL_FACE);
    glad_glEnable(GL_DEPTH_TEST);
}

void render() {
    double currentTimeInMs = glfwGetTime();
    double elapsedTime = currentTimeInMs - lastTime;
    lastTime = currentTimeInMs;

    world->update(elapsedTime);
    world->render();
}

void resize(int w,int h) {

}