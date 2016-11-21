//
//  EZGLCamera.h
//  Pods
//
//  Created by wang yang on 2016/10/22.
//
//

#import <GLKit/GLKit.h>

#import "EZGLTransform.h"
#import "EZGLNode.h"

@interface EZGLCamera : EZGLNode

@property (strong, nonatomic) EZGLTransform *transform;

- (GLKMatrix4)matrix;

@end
