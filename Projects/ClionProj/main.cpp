#include <iostream>
#include "EZGL.h"

#include <GL/glut.h>

#define VERSION_MAJOR @VERSION_MAJOR@
#define VERSION_MINOR @VERSION_MINOR@

using namespace std;

float angle = 0.1;
float cameraX = 0;
float cameraZ = 3;

static void setOrth(int w,int h) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glScaled(1, -1, 1);
    glTranslatef(0, 0, -h);

    glMatrixMode(GL_MODELVIEW);
}

static void renderBitmapString(float x, float y, float z,void *font, char *string) {
    char *c;
    glRasterPos3f(x, y, z);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

static void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(cameraX, 0, cameraZ, 0, 0, 0, 0, 1, 0);
    int time = glutGet(GLUT_ELAPSED_TIME);
    glRotated(angle, 0, 1, 0);
    glBegin(GL_TRIANGLES);
    glVertex3f(-0.5 , 0.5, 0);
    glVertex3f(-0.5, -0.5, 0);
    glVertex3f(0.5, -0.5, 0);
    glVertex3f(0.5 , -0.5, 0);
    glVertex3f(0.5, 0.5, 0);
    glVertex3f(-0.5, 0.5, 0);
    glEnd();

    renderBitmapString(0,0,0,GLUT_BITMAP_HELVETICA_18,"Hello");

    angle += 0.1;
    glutSwapBuffers();
}

static void resized(int w,int h) {
    float ratio = 1.0 * w / h;


    glPopMatrix();
    glPushMatrix();
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
}

static void processSpecialKeys(int key, int xx, int yy) {

    float fraction = 0.1f;

    switch (key) {
        case GLUT_KEY_LEFT :
            cameraX -= 0.1;
            break;
        case GLUT_KEY_RIGHT :
            cameraX += 0.1;
            break;
        case GLUT_KEY_UP :
            cameraZ -= 0.1;
            break;
        case GLUT_KEY_DOWN :
            cameraZ += 0.1;
            break;
    }
}

static void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_UP) {
            printf("Left Mouse Button is UP");
        }
    }
}

static void motion(int x,int y) {
    printf("%d-%d",x,y);
}

static void menuEvent(int menu) {
    if (menu == 2) {
        exit(0);
    }
}

static int createMenu() {
    int menu, subMenu;

    subMenu = glutCreateMenu(menuEvent);
    glutAddMenuEntry("New",3);
    glutAddMenuEntry("Exit",4);

    menu = glutCreateMenu(menuEvent);

    glutAddMenuEntry("New",1);
    glutAddMenuEntry("Exit",2);

    glutAddSubMenu("File",subMenu);

    return menu;
}

int main(int argc,char **argv) {

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
    glutReshapeFunc(resized);

    glutIdleFunc(render);
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);

    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    int menu = createMenu();
    glutAttachMenu(menu);

    glEnable(GL_DEPTH_TEST);


//    glutGameModeString("800x600:32");
//    if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
//        glutEnterGameMode();
//    } else {
//        exit(0);
//    }

    glutMainLoop();
    return 1;
}