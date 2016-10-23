//
//  EZGLCamera.h
//  Pods
//
//  Created by wang yang on 2016/10/22.
//
//

#import <GLKit/GLKit.h>

#import "EZGLTransform.h"

@interface EZGLCamera : NSObject

@property (strong, nonatomic) EZGLTransform *transform;

- (GLKMatrix4)matrix;

@end
