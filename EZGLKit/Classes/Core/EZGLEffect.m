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

- (void)applyToProgram:(EZGLProgram *)program {
    int index = 0;
    for (EZGLLight *light in self.lights) {
        NSString *name = [NSString stringWithFormat:@"lights[%d].color",index];
        glUniform4fv([program uniformWithStr:name], 1, light.color.v);
        name = [NSString stringWithFormat:@"lights[%d].position",index];
        glUniform3fv([program uniformWithStr:name], 1, light.position.v);
        name = [NSString stringWithFormat:@"lights[%d].intensity",index];
        glUniform1f([program uniformWithStr:name], light.intensity);
        index++;
    }
    glUniform1i([program uniformWithStr:@"lightNum"], self.lights.count);
}

- (void)update:(NSTimeInterval)interval {
    for (EZGLLight *light in self.lights) {
        [light update:interval];
    }
}

@end
