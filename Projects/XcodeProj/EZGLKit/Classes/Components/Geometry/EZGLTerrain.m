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

@interface EZGLTerrain() {
    float *mapData;
    uint8_t *imageData;
}

@property (strong, nonatomic) UIImage *image;
@property (assign, nonatomic) int bytesPerRow;
@property (assign, nonatomic) int bytesPerPixel;

@property (assign, nonatomic) CGSize size;
@property (assign, nonatomic) float resolution;
@property (assign, nonatomic) float maxHeight;

@property (strong, nonatomic) EZGLGeometryVertexBuffer *buffer;

@end

@implementation EZGLTerrain

- (instancetype)initWithImage:(UIImage *)image size:(CGSize)size {
    self = [super init];
    if (self) {
        self.image = image;
        self.size = image.size;
        self.maxHeight = 20;
        self.resolution = 1;
        
        CGImageRef heightMap = [self.image CGImage];
        CGContextRef context = CreateARGBBitmapContext(heightMap);
        CGContextDrawImage(context, CGRectMake(0, 0, self.size.width, self.size.height), heightMap);
        
        
        CGDataProviderRef dataProvider = CGImageGetDataProvider(heightMap);
        
        self.bytesPerRow = CGImageGetBytesPerRow(heightMap);
        self.bytesPerPixel = CGImageGetBitsPerPixel(heightMap) / CGImageGetBitsPerComponent(heightMap);
        
        imageData = CGBitmapContextGetData(context);
        
        [self setupWithData:[self genGeometryData]];
    }
    return self;
}

CGContextRef CreateARGBBitmapContext (CGImageRef inImage)
{
    CGContextRef    context = NULL;
    CGColorSpaceRef colorSpace;
    void *          bitmapData;
    int             bitmapByteCount;
    int             bitmapBytesPerRow;
    
    // Get image width, height. We'll use the entire image.
    size_t pixelsWide = CGImageGetWidth(inImage);
    size_t pixelsHigh = CGImageGetHeight(inImage);
    
    // Declare the number of bytes per row. Each pixel in the bitmap in this
    // example is represented by 4 bytes; 8 bits each of red, green, blue, and
    // alpha.
    bitmapBytesPerRow   = (pixelsWide * 4);
    bitmapByteCount     = (bitmapBytesPerRow * pixelsHigh);
    
    // Use the generic RGB color space.
    colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
    if (colorSpace == NULL)
    {
        fprintf(stderr, "Error allocating color space\n");
        return NULL;
    }
    
    // Allocate memory for image data. This is the destination in memory
    // where any drawing to the bitmap context will be rendered.
    bitmapData = malloc( bitmapByteCount );
    if (bitmapData == NULL)
    {
        fprintf (stderr, "Memory not allocated!");
        CGColorSpaceRelease( colorSpace );
        return NULL;
    }
    
    // Create the bitmap context. We want pre-multiplied ARGB, 8-bits
    // per component. Regardless of what the source image format is
    // (CMYK, Grayscale, and so on) it will be converted over to the format
    // specified here by CGBitmapContextCreate.
    context = CGBitmapContextCreate (bitmapData,
                                     pixelsWide,
                                     pixelsHigh,
                                     8,      // bits per component
                                     bitmapBytesPerRow,
                                     colorSpace,
                                     kCGImageAlphaPremultipliedFirst);
    if (context == NULL)
    {
        free (bitmapData);
        fprintf (stderr, "Context not created!");
    }
    
    // Make sure and release colorspace before returning
    CGColorSpaceRelease( colorSpace );
    
    return context;
}


- (float)yLoc:(float)xLoc zLoc:(float)zLoc {
    if (xLoc >= self.size.width) {
        xLoc = self.size.width - 1;
    }
    if (zLoc >= self.size.height) {
        zLoc = self.size.height - 1;
    }
    int loc = (int)zLoc * self.bytesPerRow + (int)xLoc * self.bytesPerPixel;
    uint8_t *pixel = &imageData[loc];
    uint8_t r = pixel[1];
    uint8_t g = pixel[2];
    uint8_t b = pixel[3];
    uint8_t a = pixel[0];
    return r / 255.0 * self.maxHeight;
}

- (void)commitChanges {
    [self setupWithData:[self genGeometryData]];
}

- (GLGeometryData)genGeometryData {
    
    if (self.buffer == nil) {
        self.buffer = [EZGLGeometryVertexBuffer new];
    } else {
        [self.buffer clear];
    }
    
    int xCount = (int)(self.size.width);
    int yCount = (int)(self.size.height);
    
    if (mapData != NULL) {
        free(mapData);
        mapData = NULL;
    }
    mapData = (float *)malloc(sizeof(float) * xCount * yCount);
    
    for (int y = 0; y < yCount; y++) {
        for (int x = 0; x < xCount; x++) {
            float xLoc = x * self.resolution - self.size.width / 2;
            float zLoc = y * self.resolution - self.size.height / 2;
            float height = [self yLoc:x zLoc:y];
            mapData[y * xCount + x] = height;
            
            GLKVector3 point1 = GLKVector3Make(xLoc, [self yLoc:x zLoc:y] - self.maxHeight / 2, zLoc);
            GLKVector3 point2 = GLKVector3Make(xLoc, [self yLoc:x zLoc:y+ self.resolution] - self.maxHeight / 2, zLoc + self.resolution);
            GLKVector3 point3 = GLKVector3Make(xLoc + self.resolution, [self yLoc:x + self.resolution zLoc:y + self.resolution] - self.maxHeight / 2, zLoc + self.resolution);
            GLKVector3 point4 = GLKVector3Make(xLoc + self.resolution, [self yLoc:x+ self.resolution zLoc:y] - self.maxHeight / 2, zLoc);
            
            float scale = 0.1;
            EZGeometryRect3 rect = {point4,point3,point2,point1,
                {scale * y,scale * x},
                {scale * y,scale * (x + 1)},
                {scale * (y + 1),scale * (x + 1)},
                {scale * (y + 1),scale * x}
            };
            [EZGLGeometryUtil appendRect:rect toVertices:self.buffer];
        }
    }
    
    [self.buffer caculatePerVertexNormal];
    
    GLfloat *vertex = (GLfloat *)[self.buffer data];
    
    GLGeometryData data;
    glGenBuffers(1, &data.vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, data.vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, [self.buffer rawLength], vertex, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    data.vertexCount = [self.buffer rawLength] / sizeof(EZGLGeometryVertex);
    data.vertexStride = sizeof(EZGLGeometryVertex);
    data.supportIndiceVBO = NO;
    return data;
}

- (NSArray *)rigidBodys {
    EZGLRigidBody *rigidBody = [[EZGLRigidBody alloc]initAsTerrain:mapData size:self.size minHeight:0 maxHeight:self.maxHeight geometry:self];
    return @[rigidBody];
}

@end
