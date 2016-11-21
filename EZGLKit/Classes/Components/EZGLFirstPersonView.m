//
//  EZGLFirstPersonView.m
//  Pods
//
//  Created by wangyang on 2016/11/21.
//
//

#import "EZGLFirstPersonView.h"
#import "EZGLPerspectiveCamera.h"
#import "EZGLNode.h"

@interface EZGLFirstPersonView ()

@property (weak, nonatomic) EZGLPerspectiveCamera *camera;
@property (weak, nonatomic) EZGLNode *node;

@end

@implementation EZGLFirstPersonView

- (instancetype)initWithCamera:(EZGLPerspectiveCamera *)camera node:(EZGLNode *)node
{
    self = [super init];
    if (self) {
        self.camera = camera;
        self.node = node;
    }
    return self;
}

- (void)update:(float)timeIntervalInSeconds {
    [self.camera translateTo:GLKVector3Make(self.node.transform.translateX,self.node.transform.translateY,self.node.transform.translateZ)];
}

@end
