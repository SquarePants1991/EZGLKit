//
//  EZGLViewController.m
//  EZGLKit
//
//  Created by ocean on 10/21/2016.
//  Copyright (c) 2016 ocean. All rights reserved.
//

#import "EZGLViewController.h"

@interface EZGLViewController () <UITableViewDelegate>
@property (strong, nonatomic) NSArray *viewControllersMap;
@end

@implementation EZGLViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    self.tableView.delegate = self;
    self.viewControllersMap = @[@"EZGLBasicViewController"];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    if (indexPath.row < self.viewControllersMap.count) {
        NSString *vcName = self.viewControllersMap[indexPath.row];
        Class cls = NSClassFromString(vcName);
        if (cls) {
            UIViewController *instance = [cls new];
            if (instance) {
                [self.navigationController pushViewController:instance animated:YES];
            }
        }
    }
}

@end
