//
//  TabBarC2PViewController.h
//  QppDemo
//
//  @brief Application Header File for Centtral to Peripheral View Controller.
//
//  Created by quintic on 4/21/14.
//  Copyright (c) 2014 Quintic. All rights reserved.
//

#import <UIKit/UIKit.h>

#define QPP_INTERVAL_PICKERVIEW_ROW_DEF          1 // 119

#define QPP_PKG_NUM_PICKERVIEW_ROW_DEF          6 // 119

#define ALERT_NOFILE_TITLE                      @"No File"

typedef enum
{
	QPP_C2P_IDLE = 0,   // send 
    QPP_C2P_DOWNLOADING,
    QPP_C2P_DOWNLOADED,
    QPP_C2P_ERROR,
} QppCent2PeriVCState;

@interface TabBarC2PViewController : UIViewController
{
    QppCent2PeriVCState QppC2PVCState;
    
    NSData *fwFileCode;
}

@property (readonly) QppCent2PeriVCState QppC2PVCState;


@property (weak, nonatomic) IBOutlet UIPickerView *qppIntervalPicker;

@property (weak, nonatomic) IBOutlet UIButton *qppC2PSendBtn;
- (IBAction)qppC2PSend:(id)sender;
@property (weak, nonatomic) IBOutlet UILabel *appC2PCountLbl;

@property (weak, nonatomic) IBOutlet UIStepper *qppPkgStepper;
- (IBAction)qppPkgNumStepper:(id)sender;
@property (weak, nonatomic) IBOutlet UILabel *qppStepperPkgNum;

- (IBAction)openFileList:(id)sender;
/// to select file start
@property (strong, nonatomic) TableViewAlert *qBlueFileSelectVC;
@property (weak, nonatomic) IBOutlet UILabel *qnLoadFileLbl;

@end
