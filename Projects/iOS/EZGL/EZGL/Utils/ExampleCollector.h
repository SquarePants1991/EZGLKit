//
//  ExampleCollector.h
//  EZGL
//
//  Created by wangyang on 2017/2/4.
//  Copyright © 2017年 wangyang. All rights reserved.
//

#import <Foundation/Foundation.h>

#define RegisterExample(name) \
+ (void)load { \
    [ExampleCollector registerExample:name vcCls:self]; \
}

@interface ExampleCollector : NSObject
+ (void)registerExample:(NSString *)name vcCls:(Class)viewControllerCls;
+ (NSMutableDictionary *)examples;
@end
