//
//  DeviceViewController.h
//  Qpp Demo
//
//  @brief Application Header File for Device List View Controller. 
//
//  Created by quintic on 5/18/14.
//  Copyright (c) 2014 Quintic. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface DeviceViewController : UIViewController{
    IBOutlet UITableView *deviceList;
}

@property (nonatomic,retain) UIRefreshControl *refreshControl;

- (IBAction)backQppPeri2CentVC : (id)sender;

-(void) updatePeriInTableView:(UIRefreshControl *)refreshControl;

+(DeviceViewController *)sharedInstance;
@end
