//
//  TabBarC2PViewController.m
//  QppDemo
//
//  @brief Application Source File for Help View Controller.
//
//  Created by quintic on 4/21/14.
//  Copyright (c) 2014 Quintic. All rights reserved.
//

#import "TabBarHelpViewController.h"

@interface TabBarHelpViewController ()

@end

@implementation TabBarHelpViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        self.title = NSLocalizedString(@"Help", @"Help");
        self.tabBarItem.image = [UIImage imageNamed:@"Help"];
    }
    return self;
}
							
- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
