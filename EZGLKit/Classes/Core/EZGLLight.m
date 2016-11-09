//
//  EZGLLight.m
//  OpenESRD
//
//  Created by wangyang on 2016/10/13.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import "EZGLLight.h"
#import "EZGLProgram.h"

@interface EZGLLight () {
    GLfloat _rawData[8];
}

@property (strong, nonatomic) EZGLPerspectiveCamera *lightCamera;

@end

@implementation EZGLLight

- (instancetype)init {
    self = [super init];
    if (self) {
        self.color = GLKVector4Make(1, 1, 1, 1);
        self.intensity = 1.5; //0 ~ 1000
        self.position = GLKVector3Make(0, 10, 10);
    }
    return self;
}

- (void)update:(NSTimeInterval)interval {
//    GLKQuaternion quaternion = GLKQuaternionMakeWithAngleAndAxis(-M_PI * interval / 10, 0, 1, 0);
//    self.position = GLKQuaternionRotateVector3(quaternion, self.position);
//    self.position = GLKVector3Make(0, self.position.y + interval, self.position.z + interval);
}

- (EZGLPerspectiveCamera *)lightCameraWithSize:(CGSize)size {
    if (self.lightCamera == nil) {
        self.lightCamera = [EZGLPerspectiveCamera cameraWithSize:size];
        self.lightCamera.transform.translateX = self.position.x;
        self.lightCamera.transform.translateY = self.position.y;
        self.lightCamera.transform.translateZ = self.position.z;
    }
    return self.lightCamera;
}

- (GLfloat *)raw {
    _rawData[0] = self.position.x;
    _rawData[1] = self.position.y;
    _rawData[2] = self.position.z;
    _rawData[3] = self.color.r;
    _rawData[4] = self.color.g;
    _rawData[5] = self.color.b;
    _rawData[6] = self.color.a;
    _rawData[7] = self.intensity;
    return _rawData;
}

- (GLsizei)rawLen {
    return sizeof(_rawData) / sizeof(GLfloat);
}

@end
