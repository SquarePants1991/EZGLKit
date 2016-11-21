//
//  EZGLNode.h
//  Pods
//
//  Created by wangyang on 2016/11/21.
//
//

#import <Foundation/Foundation.h>
#import "EZGLTransform.h"


@interface EZGLNode : NSObject

@property (strong, nonatomic) EZGLTransform *transform;

- (void)update:(float)timeIntervalInSeconds;

@end
