//
//  OtaViewController.h
//  OtaDemo
//
//  @brief Application Header File for OTA Main View Controller.
//
//  Created by quintic on 4/21/14.
//  Copyright (c) 2014 Quintic. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "QBleClient.h"
#import "otaApi.h"

#define UUID_OTA_SERVICE                 @"FEE8"

#define SCAN_DEV_TIMEOUT                 5   /// sec // max 7.5sec.
#define DIDCONN_DEV_TIMEOUT              5   /// sec
#define RESUME_TIMEOUT                   10  /// sec
#define FW_NO_RSP_TIMEOUT                12  /// sec

#define ALERT_FW_NO_RSP_TITLE            @"No Response"

/// device / connection
#define ALERT_NODEVICE_TITLE             @"No Device"
#define ALERT_NO_OTA_TITLE               @"No OTA Service"
#define ALERT_RETRIEVE_TITLE             @"Retrieve"

/// timeout
#define ALERT_CONNECT_FAIL_TITLE         @"Connection Warning"
#define ALERT_NO_CONNECT_TITLE        	 @"Connection Warning"

/// ota fail error
#define ALERT_PKG_CS_ERROR_TITLE         @"Package Checksum Error"
#define ALERT_APP_PKG_LENGTH_ERROR_TITLE @"Package Length Error"

/// meta data
#define ALERT_DEV_NOT_SUPPORT_OTA_TITLE  @"OTA Upgrade Not Allowed"
#define ALERT_APP_SIZE_ERROR_TITLE       @"Firmware Size Error"

/// verify.
#define ALERT_APP_VERIFY_ERROR_TITLE     @"Firmware Verify Error"

// firmware 
#define FW_FILE_CODE_LENGTH_MIN        0
#define FW_FILE_CODE_LENGTH_MAX        (50*1024)

#define ALERT_FILE_MIN_TITLE           @"Fimrware Size Error"
#define ALERT_FILE_MAX_TITLE           @"Fimrware Size Error"

/// for UI
#define bleScanDevEndNoti              @"bleScanPeripheralsEndNotification"

#define keyAlertDisconnect             @"key-AlertDisconnect"
#define keyAlertNoDevice               @"key-AlertNoDevice"
#define keyAlertCSError                @"key-AlertCheckSumError"
#define keyAlertFailed                 @"key-AlertFailed"
#define keyAlertRetrieve               @"key-AlertRetrieve"

#define otaRetrievePeriNoti            @"otaToRetrieveNotification"

#define otaNoDeviceNoti                @"ota-NoDeviceNotification"

#define otaPkgCheckSumErrorNoti        @"ota-CheckSumErrorNotification"
#define otaPkgLengthErrorNoti          @"ota-PkgLengthErrorNotification"

#define otaAppSizeErrorNoti            @"ota-AppSizeErrorNotification"
#define otaAppDevNotSupportOtaNoti     @"ota-AppOtaInfoInvalidNotification"

#define otaAppVerifyErrorNoti          @"ota-AppVerifyErrorNotification"

#define otaScanDevTimeoutNoti          @"ota-ScanDevTimeoutNotification"
#define otaConnectDevTimeoutNoti       @"ota-ConnectDevTimeoutNotification"
#define otaResumeTimeoutNoti           @"ota-ResumeTimeoutNotification"

#define otaNoOtaServiceNoti            @"app-NoOtaServiceNotification"
#define otaReLoadFirmwarewFileNoti     @"app-Re-LoadFirmwarewFileNotification"

#define otaFwNoRspTimeoutNoti          @"app-FirmwareNoResponseTimeoutNotification"

#define otaConnectTimeoutNoti          @"app-ConnectTimeoutNotification"

enum otaAlertBtnOptions
{
    OTA_BTN_CANCEL,
    OTA_BTN_OK,
};

typedef enum
{
	OTA_MS_IDLE = 0,  
    OTA_MS_SCANNING,
    OTA_MS_CONNECTING,
    OTA_MS_CONNECTED,
    OTA_MS_DISCONNECTING,
    OTA_MS_DISCONNECTED,
    OTA_MS_RETRIEVING,
    OTA_MS_RETRIEVED,
    OTA_MS_DOWNLOADING,
    OTA_MS_DOWNLOADED,
    OTA_MS_ERROR,
} enumOtaClientState;

@class TableViewAlert;

@interface MainViewController : UIViewController <bleDidConnectionsDelegate, otaApiUpdateAppDataDelegate> {
    
    NSData *newFwFile;
    
    NSArray *otaRspResultArr;
    NSArray *otaRspCmdArr;
    
    enumOtaClientState OTA_ClientState;
}

@property (readonly) enumOtaClientState OTA_ClientState;

@property (nonatomic,retain) UIRefreshControl *refreshControl;
@property (strong, nonatomic) IBOutlet UILabel *OtaVersion;
@property (strong, nonatomic) IBOutlet UILabel *connStatusLabel;
@property (strong, nonatomic) IBOutlet UILabel *devNameLabel;
@property (weak, nonatomic) IBOutlet UIButton *scanButton;
@property (strong, nonatomic) IBOutlet UIActivityIndicatorView *otaScanDevActInd;
@property (strong, nonatomic) IBOutlet UILabel *otaScanCountDnLbl;
@property (strong, nonatomic) IBOutlet UILabel *otaScanCountDnUnitLbl;

/// progress bar.
@property (strong, nonatomic) IBOutlet UIProgressView *otaProgressBar;
@property (strong, nonatomic) IBOutlet UILabel *otaProgressBarValue;
@property (strong, nonatomic) IBOutlet UILabel *otaProgressBarPercent;
-(void)otaHideProgressBar:(BOOL) flagHiden;

/// data rate.
@property (strong, nonatomic) IBOutlet UILabel *otaDataRateLbl;
@property (strong, nonatomic) IBOutlet UILabel *otaDataRateBpsLbl;

- (IBAction)scanPeripheral:(id)sender;

/// load file ...
@property (strong, nonatomic) IBOutlet UIButton *otaLoadFileBtn;
- (IBAction)otaOpenFwFileVC:(id)sender;

@property (nonatomic, strong) IBOutlet UILabel *otaBinFileLbl;
@property (strong, nonatomic) IBOutlet UILabel *otaLoadTimeLbl;
@property (strong, nonatomic) IBOutlet UILabel *otaLoadTimeUnitLbl;
@property (strong, nonatomic) IBOutlet UIActivityIndicatorView *otaDidConnDevActInd;

@property (strong, nonatomic) TableViewAlert *otaDisplayDevicesVC;

- (BOOL)shouldAutorOtateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation;

- (void)otaReset;

-(void)otaUserInit;

- (NSString *) otaSlaveFileName;
- (void)otaSelectedOneFileRsp:(NSNotification *)notifyFromFile;

- (void)otaDisplayPeripherals; // :(CBPeripheral *)aPeripheral;
- (void)otaSelOnePeripheralRsp :(NSNotification *)notifyFromPeripheral;

- (void)otaHideDataRate : (BOOL) flagHiden;
- (void)otaHideLoadCount : (BOOL) flagHiden;

- (void) otaScanDevTimeoutRsp;
- (void) otaStopScanDevTimeout;

- (void) otaDidConnDevTimeoutRsp;
- (void) otaStopDidConnDevTimeout;

/// resume
-(void)otaResumeReq;
-(void)otaResumeTimeoutRsp;

+(MainViewController *)sharedInstance;

@end
