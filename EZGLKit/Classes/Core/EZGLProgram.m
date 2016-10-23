//
//  OpenGLUtil.m
//  OpenESRD
//
//  Created by wang yang on 2016/10/7.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import "EZGLProgram.h"

@interface EZGLProgram () {
    int uniforms[NUM_UNIFORMS];
}

@end

@implementation EZGLProgram

- (instancetype)initWithVertexShaderFileName:(NSString *)vsName fragmentShaderFileName:(NSString *)fsName {
    self = [super init];
    if (self) {
        
        NSBundle *bundle = [NSBundle bundleForClass:self];
        
        NSString *vsFilePath = [bundle pathForResource:vsName ofType:@"vsh"];
        NSString *fsFilePath = [bundle pathForResource:fsName ofType:@"fsh"];
        self.value = [self createProgramWithVertexShaderFile:vsFilePath fragmentShaderFile:fsFilePath];
    }
    return self;
}

- (instancetype)initWithVertexShaderFile:(NSString *)vsf fragmentShaderFile:(NSString *)fsf {
    self = [super init];
    if (self) {
        self.value = [self createProgramWithVertexShaderFile:vsf fragmentShaderFile:fsf];
    }
    return self;
}

- (instancetype)initWithVertexShader:(NSString *)vs fragmentShader:(NSString *)fs {
    self = [super init];
    if (self) {
        self.value = [self createProgramWithVertexShader:vs fragmentShader:fs];
    }
    return self;
}

- (int)uniform:(int)uniformName {
    return uniforms[uniformName];
}

- (GLuint)createProgramWithVertexShaderFile:(NSString *)vsf fragmentShaderFile:(NSString *)fsf {
    NSString *vsContent = [NSString stringWithContentsOfFile:vsf encoding:NSUTF8StringEncoding error:nil];
    NSString *fsContent = [NSString stringWithContentsOfFile:fsf encoding:NSUTF8StringEncoding error:nil];
    return [self createProgramWithVertexShader:vsContent fragmentShader:fsContent];
}

- (GLuint)createProgramWithVertexShader:(NSString *)vs fragmentShader:(NSString *)fs {
    GLuint program, vertShader, fragShader;
    // Create shader program.
    program = glCreateProgram();

    const GLchar *vssource = (GLchar *)[vs UTF8String];
    const GLchar *fssource = (GLchar *)[fs UTF8String];

    if (![self compileShader:&vertShader type:GL_VERTEX_SHADER source:vssource]) {
        NSLog(@"Failed to compile vertex shader");
        return NO;
    }

    if (![self compileShader:&fragShader type:GL_FRAGMENT_SHADER source:fssource]) {
        NSLog(@"Failed to compile fragment shader");
        return NO;
    }

    // Attach vertex shader to program.
    glAttachShader(program, vertShader);

    // Attach fragment shader to program.
    glAttachShader(program, fragShader);

    // Link program.
    if (![self linkProgram:program]) {
        NSLog(@"Failed to link program: %d", program);

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

        return NO;
    }

    // Get uniform locations.
    uniforms[UNIFORM_RENDERASSHADOW] = glGetUniformLocation(program, "renderAsShadow");
    uniforms[UNIFORM_VIEWPROJECTION] = glGetUniformLocation(program, "viewProjection");
    uniforms[UNIFORM_MODEL_MATRIX] = glGetUniformLocation(program, "modelMatrix");
    uniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(program, "normalMatrix");
    uniforms[UNIFORM_AMBIENT] = glGetUniformLocation(program, "ambient");
    uniforms[UNIFORM_DIFFUSE] = glGetUniformLocation(program, "diffuse");
    uniforms[UNIFORM_SPECULAR] = glGetUniformLocation(program, "specular");
    uniforms[UNIFORM_LIGHT_VIEWPROJECTION] = glGetUniformLocation(program, "lightViewProjection");
    uniforms[UNIFORM_LIGHT_POSITION] = glGetUniformLocation(program, "lightPosition");
    uniforms[UNIFORM_LIGHT_COLOR] = glGetUniformLocation(program, "lightColor");
    uniforms[UNIFORM_LIGHT_BRIGHTNESS] = glGetUniformLocation(program, "lightBrightness");
    uniforms[UNIFORM_DIFFUSE_MAP] = glGetUniformLocation(program, "diffuseMap");
    uniforms[UNIFORM_SHADOW_MAP] = glGetUniformLocation(program, "shadowMap");

    // Release vertex and fragment shaders.
    if (vertShader) {
        glDetachShader(program, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDetachShader(program, fragShader);
        glDeleteShader(fragShader);
    }

    return program;
}

#pragma mark - Private Methods

- (BOOL)compileShader:(GLuint *)shader type:(GLenum)type source:(const GLchar *)source {
    GLint status;

    if (!source) {
        NSLog(@"Failed to load vertex shader");
        return NO;
    }

    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);

#if defined(DEBUG)
    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        NSLog(@"Shader compile log:\n%s", log);
        free(log);
    }
#endif

    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        glDeleteShader(*shader);
        return NO;
    }

    return YES;
}

- (BOOL)linkProgram:(GLuint)prog {
    GLint status;
    glLinkProgram(prog);

#if defined(DEBUG)
    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        NSLog(@"Program link log:\n%s", log);
        free(log);
    }
#endif

    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == 0) {
        return NO;
    }

    return YES;
}

- (BOOL)validateProgram:(GLuint)prog {
    GLint logLength, status;

    glValidateProgram(prog);
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        NSLog(@"Program validate log:\n%s", log);
        free(log);
    }

    glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
    if (status == 0) {
        return NO;
    }

    return YES;
}

@end
