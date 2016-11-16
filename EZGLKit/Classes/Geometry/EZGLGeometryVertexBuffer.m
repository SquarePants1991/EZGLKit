//
//  EZGLGeometryVertexBuffer.m
//  Pods
//
//  Created by wangyang on 2016/11/14.
//
//

#import "EZGLGeometryVertexBuffer.h"

@interface EZGLGeometryVertexBuffer () {
    EZGLGeometryVertex *vertices;
}

@property (assign, nonatomic) EZInt bufferLen;
@property (assign, nonatomic) EZInt index;

@end

@implementation EZGLGeometryVertexBuffer

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.bufferLen = 32;
        self.index = 0;
        vertices = (EZGLGeometryVertex *)malloc(sizeof(EZGLGeometryVertex) * self.bufferLen);
    }
    return self;
}

- (void)append:(EZGLGeometryVertex)vertex {
    *(vertices + self.index) = vertex;
    self.index++;
    if (self.index > self.bufferLen * 3 / 4.0) {
        self.bufferLen += 32;
        vertices = (EZGLGeometryVertex *)realloc(vertices, sizeof(EZGLGeometryVertex) * self.bufferLen);
    }
}

- (void)caculatePerVertexNormal {
    int sum = self.index;
    NSMutableDictionary *collectMap = [NSMutableDictionary new];
    for (int i = 0; i< sum; i++) {
        EZGLGeometryVertex vertex = *(vertices + i);
        NSString *identity = [self idFromGeometryVertex:vertex];
        NSArray *points = collectMap[identity];
        if (points == nil) {
            points = [NSArray new];
        }
        points = [points arrayByAddingObject:[NSValue valueWithPointer:(vertices + i)]];
        collectMap[identity] = points;
    }
    
    for (NSString *key in collectMap.keyEnumerator) {
        GLKVector3 normals = GLKVector3Make(0, 0, 0);
        for (NSValue *vertex in collectMap[key]) {
            EZGLGeometryVertex *pVertex = (EZGLGeometryVertex *)[vertex pointerValue];
            normals = GLKVector3Add(normals, GLKVector3Make(pVertex->nx, pVertex->ny, pVertex->nz));
        }
        normals = GLKVector3Normalize(normals);
        for (NSValue *vertex in collectMap[key]) {
            EZGLGeometryVertex *pVertex = (EZGLGeometryVertex *)[vertex pointerValue];
            pVertex->nx = normals.x;
            pVertex->ny = normals.y;
            pVertex->nz = normals.z;
        }
    }
}

- (NSString *)idFromGeometryVertex:(EZGLGeometryVertex)vertex {
    CGFloat x = floor(vertex.x * 100);
    CGFloat y = floor(vertex.y * 100);
    CGFloat z = floor(vertex.z * 100);
    return [NSString stringWithFormat:@"%.2f-%.2f-%.2f",x,y,z];
}

- (GLsizei)rawLength {
    return self.index * sizeof(EZGLGeometryVertex);
}

- (void *)data {
    return (void *)vertices;
}

- (void)clear {
    self.index = 0;
}

- (void)dealloc {
    free(vertices);
}

@end
