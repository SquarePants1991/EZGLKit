//
//  EZGLSplineCylinderGeometry.h
//  Pods
//
//  Created by wang yang on 2016/11/14.
//
//

#import <EZGLKit/EZGLKit.h>

@interface EZGLSplineCylinderGeometry : EZGLGeometry

- (instancetype)initWithHeight:(CGFloat)height radius:(CGFloat)radius segments:(CGFloat)segments ring:(CGFloat)ring;

@end
