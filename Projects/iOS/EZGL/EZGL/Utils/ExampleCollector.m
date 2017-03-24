//
//  ExampleCollector.m
//  EZGL
//
//  Created by wangyang on 2017/2/4.
//  Copyright © 2017年 wangyang. All rights reserved.
//

#import "ExampleCollector.h"

@implementation ExampleCollector
+ (void)registerExample:(NSString *)name vcCls:(Class)viewControllerCls {
    NSString *clsName = NSStringFromClass(viewControllerCls);
    [[self examples] setObject:clsName forKey:name];
}

+ (NSMutableDictionary *)examples {
    static NSMutableDictionary *examples;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        examples = [[NSMutableDictionary alloc] init];
    });
    return examples;
}
@end
