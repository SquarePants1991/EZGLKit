#include <iostream>
#include "EZGL.h"

#include <GL/glut.h>

#define VERSION_MAJOR @VERSION_MAJOR@
#define VERSION_MINOR @VERSION_MINOR@

using namespace std;

float angle = 0.1;

static void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);

    glRotated(angle, 0, 1, 0);
    glBegin(GL_TRIANGLES);
    glVertex3f(-0.5 ,0.5,0);
    glVertex3f(-0.5,-0.5,0);
    glVertex3f(0.5,0.5,0);
    glEnd();

    glutSwapBuffers();
}

static void resized(int w,int h) {
    float ratio = 1.0 * w / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45, ratio, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
}

static void processKeys(unsigned char key, int x, int y) {
    if (key == 27) {
        exit(0);
    }
    int modifier = glutGetModifiers();
    switch(key) {
        case GLUT_KEY_DOWN:
            angle += 0.1;
            break;
    }
}

int main(int argc,char **argv) {

    int windowWidth = 800;
    int windowHeight = 600;
    int centerX =  glutGet(GLUT_SCREEN_WIDTH) / 2 - windowWidth / 2;
    int centerY =  glutGet(GLUT_SCREEN_HEIGHT) / 2 - windowHeight / 2;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(centerX,centerY);
    glutInitWindowSize(windowWidth,windowHeight);
    glutCreateWindow("Demo");

    glutDisplayFunc(render);
    glutReshapeFunc(resized);

    glutIdleFunc(render);
    glutKeyboardFunc(processKeys);

    glutMainLoop();
    return 1;
}