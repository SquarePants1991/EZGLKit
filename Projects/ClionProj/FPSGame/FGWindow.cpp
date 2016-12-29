//
// Created by wangyang on 16/12/2.
//

#include "FGWindow.h"
#include <iostream>
#include <utils/ELGLState.h>
#include "FGScene.h"

FGWindow::FGWindow(GLFWwindow *glfwWindow, int width, int height) {
    this->glfwWindow = glfwWindow;
    world = new ELWorld();
    world->enablePhysics();
    int fbWidth,fbHeight;
    glfwGetFramebufferSize(glfwWindow, &fbWidth, &fbHeight);

    world->fbWidth = fbWidth;
    world->fbHeight = fbHeight;
    world->enableDefaultCamera(fbWidth / fbHeight);

    std::string vertexShader = ELFileUtil::stringContentOfShader(ELAssets::shared()->findFile("vtx_phong.glsl").c_str());
    std::string fragShader = ELFileUtil::stringContentOfShader(ELAssets::shared()->findFile("frg_phong.glsl").c_str());
    std::string shadowFragShader = ELFileUtil::stringContentOfShader(ELAssets::shared()->findFile("frg_shadowmap.glsl").c_str());
    std::string waterFragShader = ELFileUtil::stringContentOfShader(ELAssets::shared()->findFile("water.glsl").c_str());
    ELEffect * activeEffect = new ELEffect(vertexShader.c_str(), fragShader.c_str());
    ELEffect * shadowEffect = new ELEffect(vertexShader.c_str(), shadowFragShader.c_str());
    ELEffect * waterEffect = new ELEffect(vertexShader.c_str(), waterFragShader.c_str());

    activeEffect->identity = "render_scene";
    shadowEffect->identity = "gen_shadow";
    waterEffect->identity = "water";
    world->addNode(activeEffect);
    world->addNode(shadowEffect);
    world->addNode(waterEffect);

    world->addRenderPass(new ELWaterPlaneRenderPass());
    world->addRenderPass(new ELShadowMapRenderPass());

    activeEffect->frogColor = ELVector4Make(0.2,0.2,0.2,1.0);
    activeEffect->frogStart = 380;
    activeEffect->frogEnd = 500;

    ELGLState::set(GL_CULL_FACE, GL_TRUE);
    ELGLState::set(GL_DEPTH_TEST, GL_TRUE);

    mainScene = new FGScene(world);

    world->activedCamera->radiansAroundLeftLimit = ELVector2Make(-45,80);
}

void FGWindow::run() {
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(glfwWindow))
    {
        /* Render here */
        update();
        render();
        /* Swap front and back buffers */
        glfwSwapBuffers(glfwWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
}

void FGWindow::update() {
    static double lastTime = 0;
    double currentTimeInMs = glfwGetTime();
    double elapsedTime = currentTimeInMs - lastTime;
    lastTime = currentTimeInMs;

    double currentTime = glfwGetTime();
    frames++;
    if ( currentTime - lastFpsTime >= 1.0 ) { // If last cout was more than 1 sec ago
        lastFpsTime = currentTime;
        char buffer[1024];
        snprintf(buffer, 1024, "FPS: %.0f", frames);
        glfwSetWindowTitle(glfwWindow,buffer);
        frames = 0;
    }

    ELVector3 forwardDirection = world->activedCamera->forwardVector();
    forwardDirection = ELVector3MultiplyScalar(forwardDirection,walkingFactor.z);
    ELVector3 leftDirection = world->activedCamera->leftVector();
    leftDirection = ELVector3MultiplyScalar(leftDirection,walkingFactor.x);

    ELVector3 direction = ELVector3Add(forwardDirection, leftDirection);
    mainScene->playerRigidBody->setVelocityX(direction.x);
    mainScene->playerRigidBody->setVelocityZ(direction.z);

    ELQuaternion rotation = ELQuaternionMakeWithAngleAndAxis(elapsedTime / 5.0,1,0,0);


    world->update(elapsedTime);
    mainScene->update(elapsedTime);
}

void FGWindow::render() {
    // g_scene->defaultLight->position = ELQuaternionRotateVector3(rotation,  g_scene->defaultLight->position);
    world->render();
}

void FGWindow::keyPressed(int key) {
    if (key == GLFW_KEY_W) {
        walkingFactor.z = 14;
    }
    if (key == GLFW_KEY_S) {
        walkingFactor.z = -14;
    }
    if (key == GLFW_KEY_A){
        walkingFactor.x = -14;
    }
    if (key == GLFW_KEY_D){
        walkingFactor.x = 14;
    }
    if (key == GLFW_KEY_SPACE) {
        mainScene->playerRigidBody->setVelocityY(16);
    }
    if (key == GLFW_KEY_ESCAPE) {
        exit(0);
    }
}

void FGWindow::keyReleased(int key) {
    if (key == GLFW_KEY_W) {
        walkingFactor.z = 0;
        mainScene->playerRigidBody->setVelocityX(0);
        mainScene->playerRigidBody->setVelocityZ(0);
    }
    if (key == GLFW_KEY_S) {
        walkingFactor.z = 0;
        mainScene->playerRigidBody->setVelocityX(0);
        mainScene->playerRigidBody->setVelocityZ(0);
    }
    if (key == GLFW_KEY_A){
        walkingFactor.x = 0;
    }
    if (key == GLFW_KEY_D){
        walkingFactor.x = 0;
    }
}

void FGWindow::mousePressed(int mouseButton) {
    if (mouseButton == GLFW_MOUSE_BUTTON_LEFT) {
        mainScene->mouseLeftButtonClicked();
    }
    if (mouseButton == GLFW_MOUSE_BUTTON_RIGHT) {
        mainScene->mouseRightButtonClicked();
    }
}

void FGWindow::mouseReleased(int mouseButton) {

}


void FGWindow::mouseMove(int xpos, int ypos) {
    static bool isInit = false;
    if (isInit == false) {
        lastXPos = xpos;
        lastYPos = ypos;
        isInit = true;
    } else {
        double deltaX = xpos - lastXPos;
        double deltaY = ypos - lastYPos;
        world->activedCamera->rotateLookAtAroundUp(-deltaX);
        world->activedCamera->rotateLookAtAroundLeft(-deltaY);
        lastXPos = xpos;
        lastYPos = ypos;
    }
}

void FGWindow::resize(int w, int h) {
    world->activedCamera->aspect = w / (float)h;

    int fbWidth,fbHeight;
    glfwGetFramebufferSize(glfwWindow, &fbWidth, &fbHeight);
    world->fbWidth = fbWidth;
    world->fbHeight = fbHeight;
}