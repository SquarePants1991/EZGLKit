//
//  EZGLSpline.h
//  Pods
//
//  Created by wangyang on 2016/11/15.
//
//

#import <UIKit/UIKit.h>

@interface EZGLSpline : NSObject

@property (assign, nonatomic) int segments;

- (instancetype)initWithLength:(CGFloat)length segments:(int)segments;
- (void)adjust:(CGFloat)lengthLoc offset:(CGFloat)offset;
- (NSArray *)adjustOffsets;
- (CGFloat)offsetAtSegment:(int)index;
    
@end
