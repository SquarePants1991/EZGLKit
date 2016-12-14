//
//  EZGLTerrain.h
//  Pods
//
//  Created by wangyang on 2016/11/17.
//
//

#import <EZGLKit/EZGLKit.h>

@interface EZGLTerrain : EZGLGeometry

@property (assign, nonatomic) float phase;
- (instancetype)initWithImage:(UIImage *)image size:(CGSize)size;

@end
