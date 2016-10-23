//
//  GLOBJGeometry.h
//  OpenESRD
//
//  Created by wangyang on 2016/10/9.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import "EZGLGeometry.h"

@interface EZGLWaveFrontGeometry : EZGLGeometry
@property (strong, nonatomic) NSArray *geometries;

- (instancetype)initWithWaveFrontFilePath:(NSString *)path;
@end
