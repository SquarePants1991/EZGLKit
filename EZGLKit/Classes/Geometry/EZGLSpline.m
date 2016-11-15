//
//  EZGLSpline.m
//  Pods
//
//  Created by wangyang on 2016/11/15.
//
//

#import "EZGLSpline.h"

@interface EZGLSpline ()

@property (assign, nonatomic) CGFloat length;
@property (strong, nonatomic) NSMutableDictionary *adjustOffset;

@end

@implementation EZGLSpline

- (instancetype)initWithLength:(CGFloat)length segments:(int)segments
{
    self = [super init];
    if (self) {
        self.length = length;
        self.segments = segments;
        self.adjustOffset = [NSMutableDictionary new];
        for (int i=0; i<=self.segments; i++) {
            self.adjustOffset[@(i)] = @0;
        }
    }
    return self;
}

- (void)adjust:(CGFloat)lengthLoc offset:(CGFloat)offset {
    int originIndex = lengthLoc / self.length * self.segments;
    CGFloat originOffset = offset;
    CGFloat falloffRange = self.segments * 0.1;
    CGFloat falloffFactor = 1;
    
    int index = originIndex;
    CGFloat xFactor = 0;
    while (YES) {
        falloffFactor = (CGFloat)originIndex / falloffRange;
        xFactor = fabs(originIndex - index) * falloffFactor / originIndex;
        xFactor = xFactor > 1 ? 1 : xFactor;
        CGFloat originAdjust = [self.adjustOffset[@(index)] floatValue];
        originAdjust += (1 - xFactor * sin(2 * xFactor)) * offset;
        self.adjustOffset[@(index)] = @(originAdjust);
        index--;
        if (index < 0 || xFactor >= 1) {
            break;
        }
    }
    
    int originLeftIndexCount =  self.segments - originIndex;
    index = originIndex + 1;
    while (YES) {
        int leftIndexCount =  self.segments - index;
        falloffFactor = (CGFloat)originLeftIndexCount / falloffRange;
        xFactor = fabs(originLeftIndexCount - leftIndexCount) * falloffFactor / originLeftIndexCount;
        xFactor = xFactor > 1 ? 1 : xFactor;
        CGFloat originAdjust = [self.adjustOffset[@(index)] floatValue];
        originAdjust += (1 - xFactor * sin(2 * xFactor)) * offset;
        self.adjustOffset[@(index)] = @(originAdjust);
        index++;
        if (index > self.segments || xFactor >= 1) {
            break;
        }
    }
}

- (NSDictionary *)adjustOffsets {
    return [self.adjustOffset copy];
}

- (CGFloat)offsetAtSegment:(int)index {
    return [self.adjustOffset[@(index)] floatValue];
}
@end
