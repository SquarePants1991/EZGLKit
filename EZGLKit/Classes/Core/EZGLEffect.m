//
//  EZGLEffect.m
//  Pods
//
//  Created by wang yang on 2016/10/27.
//
//

#import "EZGLEffect.h"
#import "EZGLLight.h"

@implementation EZGLEffect

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.program = [[EZGLProgram alloc]initWithVertexShaderFileName:@"Default" fragmentShaderFileName:@"Default"];
        self.lights = [NSArray new];
    }
    return self;
}

- (id)initWithProgram:(EZGLProgram *)program {
    self = [super init];
    if (self) {
        self.program = program;
        self.lights = [NSArray new];
    }
    return self;
}

- (void)addLight:(EZGLLight *)light {
    self.lights = [self.lights arrayByAddingObject:light];
}

@end
