//
//  EZGLViewController.m
//  EZGLKit
//
//  Created by ocean on 10/21/2016.
//  Copyright (c) 2016 ocean. All rights reserved.
//

#import "EZGLViewController.h"
#import "ExampleCollector.h"

@interface EZGLViewController () <UITableViewDelegate,UITableViewDataSource>
@property (strong, nonatomic) NSDictionary *viewControllersMap;
@end

@implementation EZGLViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    self.tableView.delegate = self;
    self.tableView.dataSource = self;
    
    [self.tableView registerClass:[UITableViewCell class] forCellReuseIdentifier:@"Cell"];
    
    self.viewControllersMap = [[ExampleCollector examples] copy];
}

#pragma mark - Data Source

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.viewControllersMap.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"Cell"];
    cell.textLabel.text = self.viewControllersMap.allKeys[indexPath.row];
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    if (indexPath.row < self.viewControllersMap.count) {
        NSString *vcName = self.viewControllersMap[self.viewControllersMap.allKeys[indexPath.row]];
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
