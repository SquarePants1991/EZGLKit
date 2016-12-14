//
//  EZGLPhysicsWorld.h
//  OpenESRD
//
//  Created by wangyang on 2016/10/18.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import <UIKit/UIKit.h>


@class EZGLGeometry;


@interface EZGLPhysicsWorld : NSObject
- (void)update:(NSTimeInterval)interval;
- (void)createRigidbody:(float)mass geometry:(EZGLGeometry *)geometry;
- (void)updateGeometry:(EZGLGeometry *)geometry;
- (void)render:(CGRect)rect;
@end
