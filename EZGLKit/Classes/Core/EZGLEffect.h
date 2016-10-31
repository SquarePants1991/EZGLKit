//
//  EZGLEffect.h
//  Pods
//
//  Created by wang yang on 2016/10/27.
//
//

#import <Foundation/Foundation.h>
#import "EZGLProgram.h"

@class EZGLLight;

@interface EZGLEffect : NSObject

@property (strong, nonatomic) NSArray *lights;
@property (strong, nonatomic) EZGLProgram *program;

- (id)initWithProgram:(EZGLProgram *)program;
- (void)addLight:(EZGLLight *)light;
- (void)applyToProgram:(EZGLProgram *)program;
@end
