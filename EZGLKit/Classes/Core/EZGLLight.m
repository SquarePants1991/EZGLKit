//
//  EZGLLight.m
//  OpenESRD
//
//  Created by wangyang on 2016/10/13.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import "EZGLLight.h"

@implementation EZGLLight

- (instancetype)init {
    self = [super init];
    if (self) {
        self.color = GLKVector4Make(1, 1, 1, 1);
        self.brightness = 0.2; //0 ~ 1
        self.position = GLKVector3Make(7, 7, 0);
    }
    return self;
}

- (void)update:(NSTimeInterval)interval {
    GLKQuaternion quaternion = GLKQuaternionMakeWithAngleAndAxis(-M_PI * interval / 10, 0, 1, 0);
    self.position = GLKQuaternionRotateVector3(quaternion, self.position);
//    self.position = GLKVector3Make(0, self.position.y + interval, self.position.z + interval);
}

@end
