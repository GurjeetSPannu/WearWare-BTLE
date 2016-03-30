//
//  TabBarC2PViewController.m
//  QppDemo
//
//  @brief Application Source File for Centtral to Peripheral View Controller.
//
//  Created by quintic on 4/21/14.
//  Copyright (c) 2014 Quintic. All rights reserved.
//

#import "TabBarP2CViewController.h"
#import "TabBarC2PViewController.h"

#import "CustomAlertView.h"
/// #import "DeviceViewController.h"
#import "qppFileViewController.h"

#import "QppPublic.h"
#import "qnLoadFile.h"

@interface TabBarC2PViewController ()
{
    NSString *slaveBinFileName;
    
    // UIPickerView
    uint8_t qppC2PInterval; // data packageLength[1-120]
    uint8_t qppC2PPkgNum;
    
    NSTimer *appCountTimer;  /// count app processing time(s)
    uint16_t appCountTime;
    
    qppFileViewController *fileListVC;
}

@property (readwrite) QppCent2PeriVCState QppC2PVCState;

@end

@implementation TabBarC2PViewController
@synthesize QppC2PVCState;

@synthesize qppIntervalPicker;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        self.title = NSLocalizedString(@"Cent To Peri", @"CentToPeri");
        self.tabBarItem.image = [UIImage imageNamed:@"CentToPeri"];
        
        [self appResetCount];
    }
    return self;
}

- (QppCent2PeriVCState)QppP2CVCState
{
    @synchronized(self)
	{
        return QppC2PVCState;
    }
}

- (void)setQppP2CVCState : (QppCent2PeriVCState) aStatus
{
	@synchronized(self)
	{
		if (QppC2PVCState != aStatus)
		{
			QppC2PVCState = aStatus;
		}
	}
}

- (void)qppC2PInit{
    QppC2PVCState = QPP_C2P_IDLE;
    
    [qppIntervalPicker selectRow : QPP_INTERVAL_PICKERVIEW_ROW_DEF inComponent:0 animated:YES];
    qppC2PInterval = 2; // default 20 ms
    qppC2PPkgNum = 1;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    
    [self qppC2PInit];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(qppSelectedOneFileRsp:) name:strQnSelectedOneFileNoti object:nil];
    
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(qppCentral2periFileEndRsp) name:strQppSendFileEndNoti object:nil];
    
    fileListVC = [[qppFileViewController alloc] initWithNibName:@"qppFileViewController" bundle:nil];
}

#pragma mark - UIPickerViewDelegate

/**
 ****************************************************************************************
 * @brief      setup max value for picker view numberOfRowsInComponent
 *
 * @return :   the max value for the pick view
 ****************************************************************************************
 */
-(NSInteger) pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component{

    return 120;
}

/**
 ****************************************************************************************
 * @brief      setup number Of InComponent
 *
 * @return :   the number of Components (column)
 ****************************************************************************************
 */
-(NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView{
    
    return 1;
}

/**
 ****************************************************************************************
 * @brief      get the name of row
 *
 * @return :   The string of row
 ****************************************************************************************
 */
-(NSString *) pickerView : (UIPickerView *)pickerView
            titleForRow : (NSInteger)row
           forComponent : (NSInteger)component{
    // return [pickerArray objectAtIndex:row];    
    return [NSString stringWithFormat:@"%i", row + 1];
}

/**
 ****************************************************************************************
 * @brief      which row is selected
 *
 * @return :   none
 ****************************************************************************************
 */
- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
    qppC2PInterval = row + 1;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/**
 ****************************************************************************************
 * @brief      Qpp send out data on the view of CentralToPeripheral
 *
 * @return :   none
 ****************************************************************************************
 */
- (IBAction)qppC2PSend : (id)sender {
    TabBarP2CViewController *p2c = [TabBarP2CViewController sharedInstance];
    
    /// NSLog(@"QppC2PVCState : %d", QppC2PVCState);
    
    if(QppC2PVCState == QPP_C2P_IDLE)
    {
        QppC2PVCState = QPP_C2P_DOWNLOADING;
        [p2c qppLoadStart  : YES withIntvl : qppC2PInterval withPkgNum : qppC2PPkgNum];
        [_qppC2PSendBtn setTitle:@"Sending File" forState: UIControlStateNormal];
        
        [self appStartCount];
    }
    else{
        QppC2PVCState = QPP_C2P_IDLE;
        
        [p2c qppLoadStart : NO withIntvl:0 withPkgNum : 1];
        
        [_qppC2PSendBtn setTitle:@"Send File" forState: UIControlStateNormal];
        
        [self appResetCount];
    }
}

/**
 ****************************************************************************************
 * @brief      Qpp setup Number of package
 *
 * @return :   none
 ****************************************************************************************
 */
- (IBAction)qppPkgNumStepper:(id)sender {

    UIStepper *appPkgNum = (UIStepper *)sender;
    
    appPkgNum.minimumValue = 1;                 // default 0. must be less than maximumValue
    appPkgNum.maximumValue = 7;                 // default 100. must be greater than minimumValue
    appPkgNum.stepValue = 1;
    appPkgNum.wraps = YES;
    
    // NSLog(@"ptPackage Length = %f", appPkgNum.value);
    
    qppC2PPkgNum = (int)appPkgNum.value;
    
    _qppStepperPkgNum.text = [NSString stringWithFormat:@"%d", qppC2PPkgNum];
}

/**
 ****************************************************************************************
 * @brief      Qpp update count time
 ****************************************************************************************
 */
-(void)appUpdateCountTimeRsp{
    appCountTime++;
    _appC2PCountLbl.text = [NSString stringWithFormat:@"%d", appCountTime];
}

/**
 ****************************************************************************************
 * @brief      Qpp start count tiemr
 ****************************************************************************************
 */
-(void)appStartCount{
    appCountTime = 0;
    _appC2PCountLbl.text = [NSString stringWithFormat:@"%d", appCountTime];
    
    appCountTimer = [NSTimer scheduledTimerWithTimeInterval:1.0 target:self selector:@selector(appUpdateCountTimeRsp) userInfo:nil repeats:YES];
}
/**
 ****************************************************************************************
 * @brief      Qpp reset count tiemr
 ****************************************************************************************
 */
-(void)appResetCount{
    appCountTime = 0;
    _appC2PCountLbl.text = [NSString stringWithFormat:@"%d", appCountTime];
    
    [appCountTimer invalidate];
}

/**
 ****************************************************************************************
 * @brief      Qpp stop count tiemr
 ****************************************************************************************
 */
-(void)appStopCount{
    [appCountTimer invalidate];
}

/**
 ****************************************************************************************
 * @brief      Qpp open file list
 ****************************************************************************************
 */
- (IBAction)openFileList:(id)sender {    
    [[NSNotificationCenter defaultCenter] postNotificationName: reloadFileListDataNoti object:nil userInfo:nil];
    
    [self presentModalViewController : fileListVC animated:YES];
}

/**
 ****************************************************************************************
 * @brief      Qpp select a file response, it is from file list view.
 ****************************************************************************************
 */
- (void)qppSelectedOneFileRsp : (NSNotification *)notifyFromFile
{
    slaveBinFileName = [notifyFromFile.userInfo objectForKey : keyQnSelectFile];
    
    [self qBlueSelectOneFileAndLoad : slaveBinFileName ];
}

/**
 ****************************************************************************************
 * @brief  Qpp select a file and load a file, post a notification to 
 *            PeripheralToCentral View.
 ****************************************************************************************
 */
- (void)qBlueSelectOneFileAndLoad :(NSString *)fileName
{
    fwFileCode = [[qnLoadFile sharedInstance] readBinFile : fileName];    
    
    if([fwFileCode length] == 0)
    {
        CustomAlertView *qppOpenFileAlert = [[CustomAlertView alloc] initWithTitle : ALERT_NOFILE_TITLE
                                                                           message:@"No the file sent!"
                                                                          delegate:nil
                                                                 cancelButtonTitle:nil/*@"Cancel" */
                                                                 otherButtonTitles:@"OK", nil];
        [qppOpenFileAlert show];
        
        return;
    }
    
    NSDictionary *dictQppFileData = [NSDictionary dictionaryWithObject : fwFileCode forKey : keyQppFileData];
    [[NSNotificationCenter defaultCenter] postNotificationName : qppFileDataNoti object:nil userInfo : dictQppFileData];
}

/**
 ****************************************************************************************
 * @brief  Qpp file sent end response.
 ****************************************************************************************
 */
- (void)qppCentral2periFileEndRsp
{
    QppC2PVCState = QPP_C2P_IDLE;
    
    [_qppC2PSendBtn setTitle:@"Send File" forState: UIControlStateNormal];
    
    [self appStopCount];
}
@end
