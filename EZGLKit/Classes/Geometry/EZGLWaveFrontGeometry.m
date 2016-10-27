//
//  GLOBJGeometry.m
//  OpenESRD
//
//  Created by wangyang on 2016/10/9.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#import "EZGLWaveFrontGeometry.h"
#import "EZGLWaveFrontFile.h"

@interface EZGLWaveFrontGeometry ()

@property (strong, nonatomic) EZGLWaveFrontFile *obj;

@end

@implementation EZGLWaveFrontGeometry
- (instancetype)initWithWaveFrontFilePath:(NSString *)path {
    self = [super init];
    if (self) {
        self.obj = [[EZGLWaveFrontFile alloc] initWithFilePath:path];
        self.material = [EZGLMaterial new];
        NSMutableArray *geometries = [NSMutableArray new];
        for (EZGLWaveFrontShape *shape in self.obj.shapes) {
            EZGLGeometry *geometry = [EZGLGeometry new];
            GLGeometryData data;
            data.vertexCount = shape.vertexCount;
            data.vertexVBO = shape.vertexVBO;
            data.vertexStride = shape.vertexStride;
            data.supportIndiceVBO = NO;
            [geometry setupWithData:data];
            geometry.material = shape.material;
            [geometries addObject:geometry];
        }
        self.transform = [EZGLTransform new];
        self.geometries = [geometries copy];
    }
    return self;
}

- (void)prepare {
    for (EZGLGeometry *geometry in self.geometries) {
        [geometry prepare];
    }
}

- (void)setWorld:(EZGLWorld *)world {
    for (EZGLGeometry *geometry in self.geometries) {
        [geometry setWorld:world];
    }
}

- (void)draw {
    for (EZGLGeometry *geometry in self.geometries) {
        geometry.viewProjection = self.viewProjection;
        geometry.renderAsShadow = self.renderAsShadow;
        geometry.lightViewProjection = self.lightViewProjection;
//        geometry.material.diffuseMap = self.material.diffuseMap;
        geometry.material.shadowMap = self.material.shadowMap;
        geometry.transform = self.transform;
        [geometry draw];
    }
}

- (void)update:(NSTimeInterval)interval {
    for (EZGLGeometry *geometry in self.geometries) {
        [geometry update:interval];
    }
}

- (NSArray *)rigidBodys {
    NSMutableArray *bodies = [NSMutableArray new];
    
    for (EZGLGeometry *geometry in self.geometries) {
        EZGLRigidBody *body = [[EZGLRigidBody alloc]initAsSphere:1 mass:1 geometry:geometry];
//        EZGLRigidBody *body = [[EZGLRigidBody alloc]initAsStaticPlane:100 geometry:geometry];
        [bodies addObject:body];
    }
    
    return [bodies copy];
}


@end
