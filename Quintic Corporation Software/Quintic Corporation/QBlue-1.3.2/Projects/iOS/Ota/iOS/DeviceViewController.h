//
//  DeviceViewController.h
//  Ota Demo
//
//  @brief Application Header File for Device List View Controller.
//
//  Created by quintic on 5/18/14.
//  Copyright (c) 2014 Quintic. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface DeviceViewController : UIViewController
{
    IBOutlet UITableView *deviceList;
}

@property (nonatomic,retain) UIRefreshControl *refreshControl;

/**
 ****************************************************************************************
 * @brief back to OTA Main ViewController.
 ****************************************************************************************
 */
- (IBAction)backOtaMainVC:(id)sender;

-(void) refreshPeripheralsInTableView : (UIRefreshControl *)refreshControl;

+(DeviceViewController *)sharedInstance;
@end
