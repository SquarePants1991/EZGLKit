//
// Created by wangyang on 16/11/24.
//

#include "ELProgram.h"

#include <stdlib.h>

bool createProgram(const char *vertexShader, const char *fragmentShader, GLuint *pProgram);
bool compileShader(GLuint *shader, GLenum type, const GLchar *source);
bool validateProgram(GLuint prog);
bool linkProgram(GLuint prog);

ELProgram::ELProgram(const char *vertexShader, const char *fragmentShader) {
    bool result = createProgram(vertexShader, fragmentShader, &value);
    isValid = result;
}

GLuint ELProgram::uniform(char *uniformName) {
    std::string uniformNameStr = std::string(uniformName);
    if (uniformCache.find(uniformNameStr) != uniformCache.end()) {
        return uniformCache[uniformNameStr];
    }

    GLuint location = glGetUniformLocation(value, uniformName);
    uniformCache[uniformNameStr] = location;
    return location;
}


bool createProgram(const char *vertexShader, const char *fragmentShader, GLuint *pProgram) {
    GLuint program, vertShader, fragShader;
    // Create shader program.
    program = glCreateProgram();

    const GLchar *vssource = (GLchar *)vertexShader;
    const GLchar *fssource = (GLchar *)fragmentShader;

    if (!compileShader(&vertShader,GL_VERTEX_SHADER, vssource)) {
        printf("Failed to compile vertex shader");
        return false;
    }

    if (!compileShader(&fragShader,GL_FRAGMENT_SHADER, fssource)) {
        printf("Failed to compile fragment shader");
        return false;
    }

    // Attach vertex shader to program.
    glAttachShader(program, vertShader);

    // Attach fragment shader to program.
    glAttachShader(program, fragShader);

    // Link program.
    if (!linkProgram(program)) {
        printf("Failed to link program: %d", program);

        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (program) {
            glDeleteProgram(program);
            program = 0;
        }
        return false;
    }

//    // Get uniform locations.
//    uniforms[UNIFORM_RENDERASSHADOW] = glGetUniformLocation(program, "renderAsShadow");
//    uniforms[UNIFORM_VIEWPROJECTION] = glGetUniformLocation(program, "viewProjection");
//    uniforms[UNIFORM_MODEL_MATRIX] = glGetUniformLocation(program, "modelMatrix");
//    uniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(program, "normalMatrix");
//    uniforms[UNIFORM_AMBIENT] = glGetUniformLocation(program, "ambient");
//    uniforms[UNIFORM_DIFFUSE] = glGetUniformLocation(program, "diffuse");
//    uniforms[UNIFORM_SPECULAR] = glGetUniformLocation(program, "specular");
//
//    uniforms[UNIFORM_Lights] = glGetUniformLocation(program, "lights");
//    uniforms[UNIFORM_LIGHT_VIEWPROJECTION] = glGetUniformLocation(program, "lightViewProjection");
//    uniforms[UNIFORM_LIGHT_POSITION] = glGetUniformLocation(program, "lightPosition");
//    uniforms[UNIFORM_LIGHT_COLOR] = glGetUniformLocation(program, "lightColor");
//    uniforms[UNIFORM_LIGHT_BRIGHTNESS] = glGetUniformLocation(program, "lightBrightness");
//    uniforms[UNIFORM_DIFFUSE_MAP] = glGetUniformLocation(program, "diffuseMap");
//    uniforms[UNIFORM_SHADOW_MAP] = glGetUniformLocation(program, "shadowMap");

    // Release vertex and fragment shaders.
    if (vertShader) {
        glDetachShader(program, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDetachShader(program, fragShader);
        glDeleteShader(fragShader);
    }

    *pProgram = program;
    printf("Effect build success => %d \n", program);
    return true;
}


bool compileShader(GLuint *shader, GLenum type, const GLchar *source) {
    GLint status;

    if (!source) {
        printf("Failed to load vertex shader");
        return false;
    }

    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);

    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        printf("Shader compile log:\n%s", log);
        free(log);
    }

    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        glDeleteShader(*shader);
        return false;
    }

    return true;
}

bool linkProgram(GLuint prog) {
    GLint status;
    glLinkProgram(prog);

    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        printf("Program link log:\n%s", log);
        free(log);
    }

    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == 0) {
        return false;
    }

    return true;
}

bool validateProgram(GLuint prog) {
    GLint logLength, status;

    glValidateProgram(prog);
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        printf("Program validate log:\n%s", log);
        free(log);
    }

    glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
    if (status == 0) {
        return false;
    }

    return true;
}