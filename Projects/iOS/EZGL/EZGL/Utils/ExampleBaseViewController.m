//
//  ExampleBaseViewController.m
//  EZGL
//
//  Created by wangyang on 2017/2/3.
//  Copyright © 2017年 wangyang. All rights reserved.
//

#import "ExampleBaseViewController.h"

@interface ExampleBaseViewController () {
    float lastScale;
    CGPoint lastPoint;
}
@end

@implementation ExampleBaseViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    UIPinchGestureRecognizer *pinchGesture = [[UIPinchGestureRecognizer alloc]initWithTarget:self action:@selector(pinched:)];
    [self.view addGestureRecognizer:pinchGesture];
    
    UIPanGestureRecognizer *panGesture = [[UIPanGestureRecognizer alloc]initWithTarget:self action:@selector(panned:)];
    [self.view addGestureRecognizer:panGesture];
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)pinched:(UIPinchGestureRecognizer *)gesture {
    if (gesture.state == UIGestureRecognizerStateBegan) {
        lastScale = [gesture scale];
    } else if (gesture.state == UIGestureRecognizerStateChanged){
        [self userScaled:[gesture scale] - lastScale];
        lastScale = [gesture scale];
    }
}
    
    - (void)panned:(UIPanGestureRecognizer *)gesture {
        if (gesture.state == UIGestureRecognizerStateBegan) {
            lastPoint = [gesture translationInView:self.view];
        } else if (gesture.state == UIGestureRecognizerStateChanged){
            CGPoint nowPoint = [gesture translationInView:self.view];
            CGPoint dt = CGPointMake(nowPoint.x - lastPoint.x, nowPoint.y - lastPoint.y);
            [self userMoved:dt];
            lastPoint = nowPoint;
        }
    }
    
    - (void)userScaled:(float)deltaScale {
        
    }
    
    - (void)userMoved:(CGPoint)deltaPosition {
        
    }

@end
