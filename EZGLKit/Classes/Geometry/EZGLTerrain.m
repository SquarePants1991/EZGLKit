//
//  EZGLTerrain.m
//  Pods
//
//  Created by wangyang on 2016/11/17.
//
//

#import "EZGLTerrain.h"
#import "EZGLGeometryVertexBuffer.h"
#import "EZGLGeometryUtil.h"

@interface EZGLTerrain()

@property (strong, nonatomic) UIImage *image;
@property (assign, nonatomic) CGSize size;
@property (assign, nonatomic) float resolution;

@property (strong, nonatomic) EZGLGeometryVertexBuffer *buffer;

@end

@implementation EZGLTerrain

- (instancetype)initWithImage:(UIImage *)image size:(CGSize)size {
    self = [super init];
    if (self) {
        self.image = image;
        self.size = size;
        self.resolution = 1;
        [self setupWithData:[self genGeometryData]];
    }
    return self;
}

- (float)yLoc:(float)xLoc zLoc:(float)zLoc {
    return (zLoc * sin(xLoc) + xLoc * cos(zLoc)) / 50;
}

- (GLGeometryData)genGeometryData {
    self.buffer = [EZGLGeometryVertexBuffer new];
    
    int xCount = (int)(self.size.width / self.resolution);
    int yCount = (int)(self.size.height / self.resolution);
    
    for (int x = 0; x < xCount; x++) {
        for (int y = 0; y < yCount; y++) {
            float xLoc = x * self.resolution - self.size.width / 2;
            float zLoc = y * self.resolution - self.size.height / 2;
            
            GLKVector3 point1 = GLKVector3Make(xLoc, [self yLoc:xLoc zLoc:zLoc], zLoc);
            GLKVector3 point2 = GLKVector3Make(xLoc, [self yLoc:xLoc zLoc:zLoc+ self.resolution], zLoc + self.resolution);
            GLKVector3 point3 = GLKVector3Make(xLoc + self.resolution, [self yLoc:xLoc+ self.resolution zLoc:zLoc+ self.resolution], zLoc + self.resolution);
            GLKVector3 point4 = GLKVector3Make(xLoc + self.resolution, [self yLoc:xLoc+ self.resolution zLoc:zLoc], zLoc);
            
            EZGeometryRect3 rect = {point4,point3,point2,point1,
                {0,0},
            {0,1},
            {1,1},
            {1,0}
            };
            [EZGLGeometryUtil appendRect:rect toVertices:self.buffer];
        }
    }
    
    [self.buffer caculatePerVertexNormal];
    
    GLfloat *vertex = (GLfloat *)[self.buffer data];
    
    GLGeometryData data;
    glGenBuffers(1, &data.vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, data.vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, [self.buffer rawLength], vertex, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    data.vertexCount = [self.buffer rawLength] / sizeof(EZGLGeometryVertex);
    data.vertexStride = sizeof(EZGLGeometryVertex);
    data.supportIndiceVBO = NO;
    return data;
}

@end
