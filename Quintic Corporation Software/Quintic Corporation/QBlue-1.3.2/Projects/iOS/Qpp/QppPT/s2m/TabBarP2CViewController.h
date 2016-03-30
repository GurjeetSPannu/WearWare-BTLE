//
//  TabBarP2CViewController.h
//  Qpp Demo
//
//  @brief Application header file for Peripheral to Centtral View Controller.
//
//  Created by quintic on 4/21/14.
//  Copyright (c) 2014 Quintic. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "QBleClient.h"

#import "QppApi.h"

/// QPP Peripheral UUID
#define UUID_USER0_PERIPHERAL         @"UUID_QPP_PERIPHERAL"

/// QPP Service's UUID
#define UUID_USER0_SERVICE            @"0000FEE9-0000-1000-8000-00805F9B34FB"

/// QPP a Peripheral's Characteristic with CBCharacteristicPropertyRead
#define UUID_USER0_READ               @"CE01"

/// QPP a Peripheral's Characteristic with CBCharacteristicPropertyWrite
#define UUID_USER0_WRITE              @"D44BC439-ABFD-45A2-B575-925416129600"

/// QPP a Peripheral's Characteristic with CBCharacteristicPropertyNotify
#define UUID_USER0_NOTI               @"D44BC439-ABFD-45A2-B575-925416129601"

/// for user1
#define UUID_USER1_PERIPHERAL         @"UUID_USER1_PERIPHERAL"

/// read
#define UUID_USER1_SVC_FOR_READ       @"FFE5"    /// svc for write
#define UUID_USER1_CHAR_FOR_READ      @"FFE5"    /// for write

/// write
#define UUID_USER1_SVC_FOR_WRITE      @"FFE5"    /// svc for write
#define UUID_USER1_CHAR_FOR_WRITE     @"FFE9"    /// for write

/// notify
#define UUID_USER1_SVC_FOR_NOTIFY     @"FFE0"    /// svc for notify
#define UUID_USER1_CHAR_FOR_NOTIFY    @"FFE4"    /// for notify
/// indicate
#define UUID_USER1_SVC_FOR_INDICATE   @"FFE5"    /// svc for notify
#define UUID_USER1_CHAR_FOR_INDICATE  @"FFE5"    /// for notify


#define UUID_PT_PERIPHERAL            @"UUID_PT_PERIPHERAL"
#define UUID_PT_SVC_FOR_NT            UUID_QPP_SERVICE
#define UUID_PT_CHAR_FOR_NT           UUID_QPP_SC_NOTI      // for noti
#define UUID_PT_CHAR_FOR_WR           UUID_QPP_WRITE        // for write

#define _ENABLE_QPP_TEST 0

#if _ENABLE_QPP_TEST
#define UUID_QPP_PERIPHERAL        UUID_USER1_PERIPHERAL

#define UUID_QPP_SVC_FOR_READ      UUID_USER1_SVC_FOR_READ
#define UUID_QPP_CHAR_FOR_READ     UUID_USER1_CHAR_FOR_READ

#define UUID_QPP_SVC_FOR_WRITE     UUID_USER1_SVC_FOR_WRITE
#define UUID_QPP_CHAR_FOR_WRITE    UUID_USER1_CHAR_FOR_WRITE

#define UUID_QPP_SVC_FOR_NOTIFY    UUID_USER1_SVC_FOR_NOTIFY
#define UUID_QPP_CHAR_FOR_NOTIFY   UUID_USER1_CHAR_FOR_NOTIFY

#define UUID_QPP_SVC_FOR_INDICATE  UUID_USER1_SVC_FOR_INDICATE
#define UUID_QPP_CHAR_FOR_INDICATE UUID_USER1_CHAR_FOR_INDICATE

#else

//#define UUID_QPP_PERIPHERAL        UUID_USER0_PERIPHERAL
#define UUID_QPP_SVC               UUID_USER0_SERVICE

//#define UUID_QPP_SVC_FOR_READ      UUID_USER0_SERVICE      /// service for read.
//#define UUID_QPP_CHAR_FOR_READ     UUID_USER0_READ     /// char for read.

#define UUID_QPP_SVC_FOR_WRITE     UUID_USER0_SERVICE     /// service for write.
#define UUID_QPP_CHAR_FOR_WRITE    UUID_USER0_WRITE    /// char for write.

#define UUID_QPP_SVC_FOR_NOTIFY    UUID_USER0_SERVICE    /// service for notify.
#define UUID_QPP_CHAR_FOR_NOTIFY   UUID_USER0_NOTI   /// char for notify.

//#define UUID_QPP_SVC_FOR_INDICATE  UUID_USER0_SERVICE  /// service for indicate.
//#define UUID_QPP_CHAR_FOR_INDICATE UUID_USER0_NOTI /// char for indicate.
#endif

#define ALERT_DISCONNECT_TITLE              @"Disconnect Warning"
#define ALERT_NODEVICE_TITLE                @"No Device"
#define ALERT_CS_ERROR_TITLE                @"CheckSum Error Warning"
#define ALERT_FAILED_TITLE                  @"Failed Warning"
#define ALERT_RETRIEVE_TITLE                @"Retrieve"
#define ALERT_CONNECT_FAIL_TITLE            @"Connection Warning"

#define strQppScanPeriEndNoti               @"qppScanPeripheralsEndNotification"

#define strQppUpdateDataRateNoti            @"qppUpdateDataRateNotification"

#define strQppDidConnectNoti                @"bleQppDidConnectNoti"
#define strQppDidDisconnectNoti             @"bleQppDidDisconnectNoti"
#define strQppFailToConnectNoti             @"bleQppFailToConnectNoti"
#define strQppRetrievePeripheralsNoti       @"bleQppRetrievePeripheralsNoti"

#define strQppDiscoveredServicesNoti        @"bleQppDiscoveredServicesNoti"

#define strQppUpdateStateForCharNoti        @"bleQppUpdateStateForCharNoti"

#define strQppReceiveDataNoti               @"bleQppUpdateValueForCharNoti"

/// UI noti
#define strQppSendFileEndNoti               @"qppSendFileEndNotification"

typedef enum
{
	QPP_P2C_IDLE = 0, // scan
    QPP_P2C_SCANNING,
    QPP_P2C_SCANNED,
    QPP_P2C_CONNECTING,
    QPP_P2C_CONNECTED,
    QPP_P2C_DISCONNECTING,
    QPP_P2C_DISCONNECTED,
    QPP_P2C_RETRIEVING,
    QPP_P2C_RETRIEVED,
    QPP_P2C_ERROR,
} QppPeri2CentVCState;

@class TableViewAlert;

@interface TabBarP2CViewController : UIViewController <bleDidConnectionsDelegate, qppReceiveDataDelegate>
{
    QppPeri2CentVCState QppP2CVCState;
}

@property (readonly) QppPeri2CentVCState QppP2CVCState;

@property (strong, nonatomic) IBOutlet UILabel *connStatusLabel;
@property (strong, nonatomic) IBOutlet UILabel *devNameLabel;

@property (weak, nonatomic) IBOutlet UIButton *connectButton;

@property (strong, nonatomic) IBOutlet UILabel *ptScanCountDnLbl;
@property (strong, nonatomic) IBOutlet UILabel *ptScanCountDnUnitLbl;
@property (weak, nonatomic) IBOutlet UIActivityIndicatorView *ptScanDevActInd;

@property (strong, nonatomic) TableViewAlert *ptDisDevicesVC;
@property (strong, nonatomic) IBOutlet UIActivityIndicatorView *ptDidConnDevActInd;

- (IBAction)ScanPeri:(id)sender;

/// p2c
@property (weak, nonatomic) IBOutlet UILabel *qppPtErrorData;
@property (weak, nonatomic) IBOutlet UILabel *qppPtErrorCounter;

@property (weak, nonatomic) IBOutlet UILabel *qppReceivedData;
@property (weak, nonatomic) IBOutlet UILabel *qppReceivedDataFromChar;

@property (weak, nonatomic) IBOutlet UILabel *qppDataRateCurLbl; /// current data rate

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation;

- (void)qppLoadStart: (BOOL)onOff  withIntvl : (uint8_t)loadInterval withPkgNum:(uint8_t)pkgNum;

+ (TabBarP2CViewController *)sharedInstance;

@end