//
// Created by wangyang on 16/11/24.
//

#include "EZGL.h"

void init();
void render();
void resize(GLFWwindow *window ,int w,int h);
void setupWindow(int argc,char **argv);
void key_callback(GLFWwindow*,int,int,int,int);

ELWorld *world;
double lastTime = 0;
const float WindowWidth = 800;
const float WindowHeight = 600;
ELGameObject *player;
ELRigidBody *playerRigidBody;
ELLight * defaultLight;

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
    glfwWindowHint(GLFW_SAMPLES, 3);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WindowWidth, WindowHeight, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

//    glfwSetWindowPos(window,0,1920 - WindowHeight);
        glfwSetWindowPos(window,0,0);

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
    glfwSetWindowSizeCallback(window,resize);
    glfwSetKeyCallback(window, key_callback);

    init();

    int fbWidth,fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    world->fbWidth = fbWidth;
    world->fbHeight = fbHeight;

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

    ELAssets::shared()->addSearchPath("/Users/ocean/Documents/Codes/On Git/EZGLKit/Projects/Clion/");
    ELAssets::shared()->addSearchPath("/Users/ocean/Documents/Codes/On Git/EZGLKit/Projects/Clion/Shader/");
    ELAssets::shared()->addSearchPath("/Users/wangyang/Documents/Projects/On Git/EZGLKit/Projects/Clion/");
    ELAssets::shared()->addSearchPath("/Users/wangyang/Documents/Projects/On Git/EZGLKit/Projects/Clion/Shader/");

    std::string vertexShader = ELFileUtil::stringContentOfFile(ELAssets::shared()->findFile("vertex.glsl").c_str());
    std::string fragShader = ELFileUtil::stringContentOfFile(ELAssets::shared()->findFile("frag.glsl").c_str());
    std::string shadowFragShader = ELFileUtil::stringContentOfFile(ELAssets::shared()->findFile("shadow_frag.glsl").c_str());
    ELEffect * activeEffect = new ELEffect(vertexShader.c_str(), fragShader.c_str());
    ELEffect * shadowEffect = new ELEffect(vertexShader.c_str(), shadowFragShader.c_str());

    activeEffect->identity = "default";
    activeEffect->identity = "shadow";
    world->addNode(activeEffect);
    world->addNode(shadowEffect);

    defaultLight = new ELLight();
    defaultLight->position = ELVector3Make(0,8,0);
    defaultLight->color = ELVector4Make(1.0,1.0,1.0,1.0);
    defaultLight->intensity = 1.0;
    world->addNode(defaultLight);

    ELGameObject *gameObject = new ELGameObject(world);
//    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0, 4.5, 0);
    ELCubeGeometry *cube = new ELCubeGeometry(ELVector3Make(1,1,1));
    gameObject->addComponent(cube);
    cube->material.diffuse = ELVector4Make(1.0,0.0,0.0,1.0);
//    cube->material.diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("rock.png"))->value;
//    cube->material.normalMap = ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;

    player = gameObject;

    ELCollisionShape *collisionShape = new ELCollisionShape();
    collisionShape->asBox(ELVector3Make(0.5,0.5,0.5));
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape,1.0);
    gameObject->addComponent(rigidBody);
    rigidBody->setVelocity(ELVector3Make(0, 0, 10));
    playerRigidBody = rigidBody;


    ELGameObject *gameObject2 = new ELGameObject(world);
    world->addNode(gameObject2);
    gameObject2->transform->position = ELVector3Make(0, 0, 0);
    ELCubeGeometry *plane = new ELCubeGeometry(ELVector3Make(80,1,80));
    gameObject2->addComponent(plane);
    plane->material.diffuse = ELVector4Make(1.0,1.0,1.0,1.0);
//   plane->material.diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("rock.png"))->value;
   plane->material.normalMap = ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;

    ELCollisionShape *collisionShape2 = new ELCollisionShape();
    collisionShape2->asBox(ELVector3Make(40,0.5,40));
    ELRigidBody *rigidBody2 = new ELRigidBody(collisionShape2,0.0);
    gameObject2->addComponent(rigidBody2);

//    ELGameObject *gameObject3 = new ELGameObject(world);
//    world->addNode(gameObject3);
//    gameObject3->transform->position = ELVector3Make(0, 0, -4);
//    gameObject3->transform->quaternion = ELQuaternionMakeWithAngleAndAxis(M_PI / 2,1,0,0);
//    ELPlaneGeometry *plane2 = new ELPlaneGeometry(ELVector2Make(50,50));
//    gameObject3->addComponent(plane);
//    gameObject3->addComponent(effect);
//    plane2->material.diffuse = ELVector4Make(1.0,0.0,0.0,1.0);

    std::vector<ELMeshGeometry *> geometries =  ELWaveFrontLoader::loadFile(ELAssets::shared()->findFile("scene3.obj"));
    for (int i = 0; i < geometries.size(); ++i) {
        ELGameObject *gameObjectMesh = new ELGameObject(world);
        world->addNode(gameObjectMesh);
        gameObjectMesh->addComponent(geometries.at(i));
//        gameObjectMesh->addComponent(effect);
        gameObjectMesh->transform->position = ELVector3Make( 0 , 0 , 0);
        geometries.at(i)->material.diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("rock.png"))->value;
        geometries.at(i)->material.normalMap = ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;
    }

    world->mainCamera->lockOn(gameObject->transform);

    glad_glEnable(GL_CULL_FACE);
    glad_glDepthMask(GL_TRUE);
    glad_glDepthFunc(GL_LESS);
    glad_glEnable(GL_DEPTH_TEST);
}

void render() {
    double currentTimeInMs = glfwGetTime();
    double elapsedTime = currentTimeInMs - lastTime;
    lastTime = currentTimeInMs;

    defaultLight->position = ELVector3Make(defaultLight->position.x + elapsedTime,defaultLight->position.y,defaultLight->position.z + elapsedTime);

    world->update(elapsedTime);
    world->render();
}

void resize(GLFWwindow *window ,int w,int h) {
    world->mainCamera->aspect = w / (float)h;

    int fbWidth,fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    world->fbWidth = fbWidth;
    world->fbHeight = fbHeight;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_UP) {
            playerRigidBody->setVelocityZ(-15);
        }
        if (key == GLFW_KEY_DOWN) {
            playerRigidBody->setVelocityZ(5);
        }
        if (key == GLFW_KEY_LEFT){
            playerRigidBody->setVelocityX(-15);
        }
        if (key == GLFW_KEY_RIGHT){
            playerRigidBody->setVelocityX(5);
        }
    }

    if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_UP) {
            playerRigidBody->setVelocityZ(0);
        }
        if (key == GLFW_KEY_DOWN) {
            playerRigidBody->setVelocityZ(0);
        }
        if (key == GLFW_KEY_LEFT){
            playerRigidBody->setVelocityX(0);
        }
        if (key == GLFW_KEY_RIGHT){
            playerRigidBody->setVelocityX(0);
        }
    }
}