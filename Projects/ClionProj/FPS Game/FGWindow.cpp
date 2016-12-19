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
const float WindowHeight = 228;
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
//    glfwWindowHint(GLFW_SAMPLES, 3);

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
//        glfwSetWindowPos(glfwWindow,0,0);

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

    ELAssets::shared()->addSearchPath("/Users/ocean/Documents/Codes/On Git/EZGLKit/Projects/ClionProj/");
    ELAssets::shared()->addSearchPath("/Users/ocean/Documents/Codes/On Git/EZGLKit/Projects/ClionProj/Shader/");
    ELAssets::shared()->addSearchPath("/Users/ocean/Documents/Codes/On Git/EZGLKit/Projects/ClionProj/FPS Game/Textures/");
    ELAssets::shared()->addSearchPath("/Users/wangyang/Documents/Projects/On Git/EZGLKit/Projects/ClionProj/");
    ELAssets::shared()->addSearchPath("/Users/wangyang/Documents/Projects/On Git/EZGLKit/Projects/ClionProj/Shader/");
    ELAssets::shared()->addSearchPath("/Users/wangyang/Documents/Projects/On Git/EZGLKit/Projects/ClionProj/FPS Game/Textures/");

    std::string vertexShader = ELFileUtil::stringContentOfFile(ELAssets::shared()->findFile("vtx_phong.glsl").c_str());
    std::string fragShader = ELFileUtil::stringContentOfFile(ELAssets::shared()->findFile("frg_phong.glsl").c_str());
    std::string shadowFragShader = ELFileUtil::stringContentOfFile(ELAssets::shared()->findFile("frg_shadowmap.glsl").c_str());
    std::string waterFragShader = ELFileUtil::stringContentOfFile(ELAssets::shared()->findFile("water.glsl").c_str());
    ELEffect * activeEffect = new ELEffect(vertexShader.c_str(), fragShader.c_str());
    ELEffect * shadowEffect = new ELEffect(vertexShader.c_str(), shadowFragShader.c_str());
    ELEffect * waterEffect = new ELEffect(vertexShader.c_str(), waterFragShader.c_str());

    activeEffect->identity = "render_scene";
    shadowEffect->identity = "gen_shadow";
    waterEffect->identity = "water";
    world->addNode(activeEffect);
    world->addNode(shadowEffect);
    world->addNode(waterEffect);

    glad_glEnable(GL_CULL_FACE);
    glad_glDepthMask(GL_TRUE);
    glad_glDepthFunc(GL_LESS);
    glad_glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
    ELVector3 direction = g_world->activedCamera->forward();
    direction = ELVector3MultiplyScalar(direction,walkingFactor);
    g_scene->playerRigidBody->setVelocityX(direction.x);
    g_scene->playerRigidBody->setVelocityZ(direction.z);

    world->update(elapsedTime);

    ELVector3 start = g_world->activedCamera->position();
    ELVector3 end = ELVector3Add(start,ELVector3MultiplyScalar(g_world->activedCamera->forward(),100));
    std::vector<ELGameObject *> objs = ELPhysicsWorld::shared()->raycast(start,end);
    for (int i = 0; i < objs.size(); ++i) {
        ELGameObject *obj = objs.at(i);
        std::vector<ELNode *> nodes = obj->findChildrenWithKind("geometry");
        for (int j = 0; j < nodes.size(); ++j) {
            ELGeometry *geo = (ELGeometry *)nodes.at(j);
            geo->enableBorder = true;
        }
    }
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
            walkingFactor = 8;
        }
        if (key == GLFW_KEY_DOWN) {
            walkingFactor = -8;
        }
        if (key == GLFW_KEY_LEFT){
            rotateUpFactor = 70;
        }
        if (key == GLFW_KEY_RIGHT){
            rotateUpFactor = -70;
        }
        if (key == GLFW_KEY_SPACE) {
            g_scene->playerRigidBody->setVelocityY(5);
        }
        if (key == GLFW_KEY_ESCAPE) {
            exit(0);
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