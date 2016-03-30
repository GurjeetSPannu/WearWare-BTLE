//
//  FwFileViewController.h
//  Ota Demo
//
//  @brief Application Header File for File List View Controller.
//
//  Created by quintic on 5/18/14.
//  Copyright (c) 2014 Quintic. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface FwFileViewController : UIViewController
{
    __weak IBOutlet UITableView *fwFileList;
}

/**
 ****************************************************************************************
 * @brief back to OTA Main ViewController.
 ****************************************************************************************
 */
- (IBAction)backOtaMainVC:(id)sender;

@end
