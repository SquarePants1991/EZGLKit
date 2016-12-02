//
// Created by wangyang on 16/12/2.
//

#include "FGWindow.h"
#include <iostream>
#include "FGScene.h"


ELWorld *g_world;
FGScene *g_scene;
double lastTime = 0;
float rotateUpFactor = 0;
float walkingFactor = 0;
const float WindowWidth = 400;
const float WindowHeight = 200;
ELGameObject *player;
ELRigidBody *playerRigidBody;
ELLight * defaultLight;

bool isWalking = false;

void init();
void render(ELWorld *world);
void resize(GLFWwindow *window ,int w,int h);
void setupWindow(int argc,char **argv);
void key_callback(GLFWwindow*,int,int,int,int);
void focus_callback(GLFWwindow*,int);

FGWindow::FGWindow() {

    world = new ELWorld(WindowWidth / WindowHeight);
    /* Initialize the library */
    if (!glfwInit()) {
        printf("GLFW Init Failed!!!");
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 3);

    /* Create a windowed mode window and its OpenGL context */
    glfwWindow = glfwCreateWindow(WindowWidth, WindowHeight, "Hello World", NULL, NULL);
    if (!glfwWindow)
    {
        printf("GLFW Window create failed!!!");
        glfwTerminate();
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(glfwWindow);

    glfwSetWindowPos(glfwWindow,0,1920 - WindowHeight);
//        glfwSetWindowPos(window,0,0);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return;
    }

    printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
    if (GLVersion.major < 2) {
        printf("Your system doesn't support OpenGL >= 2!\n");
        return;
    }

    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
           glGetString(GL_SHADING_LANGUAGE_VERSION));

    glfwSwapInterval(1);
    glfwSetWindowSizeCallback(glfwWindow,resize);
    glfwSetKeyCallback(glfwWindow, key_callback);
    glfwSetWindowFocusCallback(glfwWindow,focus_callback);

    int fbWidth,fbHeight;
    glfwGetFramebufferSize(glfwWindow, &fbWidth, &fbHeight);

    world->fbWidth = fbWidth;
    world->fbHeight = fbHeight;

    ELAssets::shared()->addSearchPath("/Users/ocean/Documents/Codes/On Git/EZGLKit/Projects/Clion/");
    ELAssets::shared()->addSearchPath("/Users/ocean/Documents/Codes/On Git/EZGLKit/Projects/Clion/Shader/");
    ELAssets::shared()->addSearchPath("/Users/wangyang/Documents/Projects/On Git/EZGLKit/Projects/Clion/");
    ELAssets::shared()->addSearchPath("/Users/wangyang/Documents/Projects/On Git/EZGLKit/Projects/Clion/Shader/");

    std::string vertexShader = ELFileUtil::stringContentOfFile(ELAssets::shared()->findFile("vertex.glsl").c_str());
    std::string fragShader = ELFileUtil::stringContentOfFile(ELAssets::shared()->findFile("frag.glsl").c_str());
    std::string shadowFragShader = ELFileUtil::stringContentOfFile(ELAssets::shared()->findFile("shadow_frag.glsl").c_str());
    ELEffect * activeEffect = new ELEffect(vertexShader.c_str(), fragShader.c_str());
    ELEffect * shadowEffect = new ELEffect(vertexShader.c_str(), shadowFragShader.c_str());

    activeEffect->identity = "render_scene";
    shadowEffect->identity = "gen_shadow";
    world->addNode(activeEffect);
    world->addNode(shadowEffect);

    glad_glEnable(GL_CULL_FACE);
    glad_glDepthMask(GL_TRUE);
    glad_glDepthFunc(GL_LESS);
    glad_glEnable(GL_DEPTH_TEST);

    mainScene = new FGScene(world);

    g_world = world;
    g_scene = mainScene;
}

void FGWindow::run() {
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(glfwWindow))
    {
        /* Render here */
        glad_glClearColor(1.0, 1.0, 0.0, 1.0);
        glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render(world);
        /* Swap front and back buffers */
        glfwSwapBuffers(glfwWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
}

void render(ELWorld *world) {
    double currentTimeInMs = glfwGetTime();
    double elapsedTime = currentTimeInMs - lastTime;
    lastTime = currentTimeInMs;

    g_world->activedCamera->rotateLookAtAroundUp(elapsedTime * rotateUpFactor);
    g_scene->playerRigidBody->applyForce(ELVector3MultiplyScalar(g_world->activedCamera->forward(), walkingFactor),ELVector3Make(0,0,0));
//    ELVector3 speed = ELVector3MultiplyScalar(g_world->activedCamera->forward(), walkingFactor);
//    g_scene->playerRigidBody->setVelocityX(speed.x);
//    g_scene->playerRigidBody->setVelocityZ(speed.z);

    world->update(elapsedTime);
    world->render();
}

void resize(GLFWwindow *window ,int w,int h) {
    g_world->activedCamera->aspect = w / (float)h;

    int fbWidth,fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    g_world->fbWidth = fbWidth;
    g_world->fbHeight = fbHeight;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_UP) {
            walkingFactor = 7.0;
        }
        if (key == GLFW_KEY_DOWN) {
            walkingFactor = -7.0;
        }
        if (key == GLFW_KEY_LEFT){
            rotateUpFactor = 70;
        }
        if (key == GLFW_KEY_RIGHT){
            rotateUpFactor = -70;
        }
    }

    if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_UP) {
            walkingFactor = 0;
            g_scene->playerRigidBody->setVelocityX(0);
            g_scene->playerRigidBody->setVelocityZ(0);
        }
        if (key == GLFW_KEY_DOWN) {
            walkingFactor = 0;
            g_scene->playerRigidBody->setVelocityX(0);
            g_scene->playerRigidBody->setVelocityZ(0);
        }
        if (key == GLFW_KEY_LEFT){
            rotateUpFactor = 0;
        }
        if (key == GLFW_KEY_RIGHT){
            rotateUpFactor = 0;
        }
    }
}

void focus_callback(GLFWwindow *window, int focus) {

}