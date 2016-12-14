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

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
        return -1;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glad_glClearColor(1.0, 1.0, 0.0, 1.0);
        glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 1;
}