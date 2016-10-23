//
//  GLPhysicsWorld.h
//  OpenESRD
//
//  Created by wangyang on 2016/10/18.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import <UIKit/UIKit.h>


@class GLGeometry;


@interface GLPhysicsWorld : NSObject
- (void)update:(NSTimeInterval)interval;
- (void)createRigidbody:(float)mass geometry:(GLGeometry *)geometry;
- (void)updateGeometry:(GLGeometry *)geometry;
- (void)render:(CGRect)rect;
@end
