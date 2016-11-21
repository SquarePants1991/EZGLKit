//
//  EZGLFirstPersonView.h
//  Pods
//
//  Created by wangyang on 2016/11/21.
//
//

#import <Foundation/Foundation.h>
#import "EZGLNode.h"
#import "EZGLPerspectiveCamera.h"

@interface EZGLFirstPersonView : EZGLNode

- (instancetype)initWithCamera:(EZGLPerspectiveCamera *)camera node:(EZGLNode *)node;

@end
