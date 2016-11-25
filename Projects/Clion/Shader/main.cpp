//
// Created by wangyang on 16/11/24.
//

#include "EZGL.h"

void render();
void resize(int w,int h);
void setupWindow(int argc,char **argv);

ELWorld *world;
int lastTime = 0;

int main(int argc,char **argv) {
    setupWindow(argc, argv);

    // init game world
    world = new ELWorld();

    std::string vertexShader = ELFileUtil::stringContentOfFile("/Users/ocean/Documents/Codes/On Git/EZGLKit/Projects/Clion/Shader/vertex.glsl");
    std::string fragShader = ELFileUtil::stringContentOfFile("/Users/ocean/Documents/Codes/On Git/EZGLKit/Projects/Clion/Shader/frag.glsl");
    ELEffect *effect = new ELEffect(vertexShader.c_str(), fragShader.c_str());

    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);

    ELPlanGeometry *plan = new ELPlanGeometry(ELVector2Make(100,100));
    gameObject->addComponent(plan);
    gameObject->addComponent(effect);

    glutMainLoop();
    return 1;
}

void setupWindow(int argc,char **argv) {

    glutInitContextVersion(3,2);
    glutInitContextFlags(GLUT_CORE_PROFILE);

    int windowWidth = 100;
    int windowHeight = 100;
    int centerX =  glutGet(GLUT_SCREEN_WIDTH) / 2 - windowWidth / 2;
    int centerY =  glutGet(GLUT_SCREEN_HEIGHT) / 2 - windowHeight / 2;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(centerX,centerY);
    glutInitWindowSize(windowWidth,windowHeight);
    glutCreateWindow("Demo");

    glutDisplayFunc(render);
    glutReshapeFunc(resize);
    glutIdleFunc(render);

    glEnable(GL_DEPTH_TEST);
}

void render() {
    int currentTimeInMs = glutGet(GLUT_ELAPSED_TIME);
    int elapsedTime = currentTimeInMs - lastTime;
    lastTime = currentTimeInMs;

    world->update(elapsedTime / 1000.0f);
    world->render();

    glutSwapBuffers();
}

void resize(int w,int h) {

}