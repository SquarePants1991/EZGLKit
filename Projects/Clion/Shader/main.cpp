//
// Created by wangyang on 16/11/24.
//

#include "EZGL.h"

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

//    glutDisplayFunc(render);
//    glutReshapeFunc(resized);
//
//    glutIdleFunc(render);
//    glutKeyboardFunc(processKeys);
//    glutSpecialFunc(processSpecialKeys);
//
//    glutMouseFunc(mouse);
//    glutMotionFunc(motion);
//
//    int menu = createMenu();
//    glutAttachMenu(menu);

    glEnable(GL_DEPTH_TEST);

    std::string vertexShader = ELFileUtil::stringContentOfFile("/Users/ocean/Documents/Codes/On Git/EZGLKit/Projects/Clion/Shader/vertex.glsl");
    std::string fragShader = ELFileUtil::stringContentOfFile("/Users/ocean/Documents/Codes/On Git/EZGLKit/Projects/Clion/Shader/frag.glsl");
    ELProgram *program = new ELProgram(vertexShader.c_str(), fragShader.c_str());


//    glutGameModeString("800x600:32");
//    if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
//        glutEnterGameMode();
//    } else {
//        exit(0);
//    }

    glutMainLoop();
    return 1;
}