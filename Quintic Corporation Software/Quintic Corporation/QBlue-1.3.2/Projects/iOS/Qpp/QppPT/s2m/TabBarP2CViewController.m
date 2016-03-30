//
//  TabBarP2CViewController.m
//  Qpp Demo
//
//  @brief Application source file for Peripheral to Centtral View Controller.
//
//  Created by quintic on 4/21/14.
//  Copyright (c) 2014 Quintic. All rights reserved.
//

#import "QppPublic.h"
#import "QBleClient.h"
#import "QppApi.h"

#import "CustomAlertView.h"
#import "qnLoadFile.h"

#import "DeviceViewController.h"
#import "TabBarP2CViewController.h"

#define QPP_SCAN_DEV_TIMEOUT         5
#define QPP_DIDCONN_DEV_TIMEOUT      5


/// test transfer data
#define QPP_PKG_UNIT_LENGTH 20
static uint8_t sendData[QPP_PKG_UNIT_LENGTH] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};

static int8_t preData = 0;

static int32_t ptErrorCounter = 0;

static uint16_t fileDataIdx = 0;
static uint16_t fileLength = 0;

static const int8_t *fileDataBuf;

CBPeripheral        *qppConnectedPeri;

@interface TabBarP2CViewController () {
    NSTimer *qppScanDevTimeoutTimer;
    uint8_t  qppScanDevTimeoutCount;
    
    NSTimer *qppDidConnDevTimeoutTimer;
    uint8_t  qppDidConnDevTimeoutCount;
    
    NSTimer *qppDidSendOnTimeTimer;
    
    /// data rate
    int8_t dataRateStart;
    BOOL   flagDataMonitoring;
    uint16_t qppDataRateMin;
    uint16_t qppDataRateMax;
    
    /// cent 2 peri
    int8_t qppC2pPkgNum;
    
    NSData *fileData;
    
    DeviceViewController *deviceVC;
    
// @public
    u_int64_t preTimeMs, curTimeMs;
}

@property (readwrite) QppPeri2CentVCState QppP2CVCState;
@end

@implementation TabBarP2CViewController

@synthesize QppP2CVCState;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle : nibBundleOrNil];
    
    if (self) {
        self.title = NSLocalizedString(@"Peri To Cent", @"PeriToCent");
        self.tabBarItem.image = [UIImage imageNamed:@"PeriToCent"];
    }
    return self;
}

- (QppPeri2CentVCState)QppP2CVCState
{
    @synchronized(self)
	{
        return QppP2CVCState;
    }
}

- (void)setQppP2CVCState : (QppPeri2CentVCState) aStatus
{
	@synchronized(self)
	{
		if (QppP2CVCState != aStatus)
		{
			QppP2CVCState = aStatus;
		}
	}
}

+ (TabBarP2CViewController *)sharedInstance
{
    static TabBarP2CViewController *_sharedInstance = nil;
    
    if (_sharedInstance == nil) {
        _sharedInstance = [[TabBarP2CViewController alloc] init];
    }
    
    return _sharedInstance;
}

-(void)qppReset{
    dataRateStart = 0;
    flagDataMonitoring = false;
    
    preTimeMs = 0l;
    curTimeMs = 0l;
    
    qppDataRateMin = 10000;
    qppDataRateMax = 0;
}

/*
 * description : user config
 *
 */
-(void)qppUserConfig
{
    [[QppApi sharedInstance] qppRegUUIDs : UUID_QPP_SVC withWrChar : UUID_QPP_CHAR_FOR_WRITE];

    /// Note : Please setup for qBleClient connections update delegate. 
    [qBleClient sharedInstance].bleDidConnectionsDelegate = self;
    
    /// Note : Please setup for QppApi update delegate. 
    [QppApi sharedInstance].ptReceiveDataDelegate = self; 
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    _ptScanDevActInd.hidesWhenStopped = YES;
    _ptDidConnDevActInd.hidesWhenStopped = YES;
    
    [self.ptScanDevActInd stopAnimating];
    [self.ptDidConnDevActInd stopAnimating];
    
    QppP2CVCState = QPP_P2C_IDLE;
    
    [self refreshConnectBtns];
    
    _ptScanCountDnLbl.hidden = YES;
    _ptScanCountDnUnitLbl.hidden = YES;
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(qppDidDiscoveredServicesRsp) name: bleDiscoveredServicesNoti object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(qppDidDiscoveredCharsRsp) name: bleDiscoveredCharacteristicsNoti object:nil];
    
    /// update data
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(qppUpdateStateForCharRsp:) name: strQppUpdateStateForCharNoti object:nil];
    
    /// UI
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(qppDisplayPeripherals) name:strQppScanPeriEndNoti object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(qppUpdateDataRate) name:strQppUpdateDataRateNoti object:nil];
    
    /// open file
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(qppGetFileDataRsp:) name : qppFileDataNoti object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(qppSelOnePeripheralRsp:) name : qppSelOnePeripheralNoti object:nil];
    
    /// load Device list VC
    deviceVC = [[DeviceViewController alloc] initWithNibName:@"DeviceViewController" bundle:nil];
}

- (void)viewDidUnload {
    [self setDevNameLabel:nil];
    [self setConnStatusLabel:nil];
    [super viewDidUnload];
}

#pragma mark - the delegate from QBleClient
/**
 ****************************************************************************************
 * @brief       delegate ble update connected peripheral.
 *
 * @param[out]  aPeripheral : the connected peripheral.
 *
 ****************************************************************************************
 */
-(void)bleDidConnectPeripheral : (CBPeripheral *)aPeripheral{
    
     QppP2CVCState = QPP_P2C_CONNECTED;
     
     [self qppStopDidConnDevTimeout];
     [self refreshConnectBtns];
}

/**
 ****************************************************************************************
 * @brief       delegate ble update disconnected peripheral.
 *
 * @param[out]  aPeripheral : the disconnected peripheral.
 * @param[out]  error
 *
 ****************************************************************************************
 */
-(void)bleDidDisconnectPeripheral : (CBPeripheral *)aPeripheral error : (NSError *)error{
    
    QppP2CVCState = QPP_P2C_IDLE;
    
    [self qppStopDidConnDevTimeout];
    
    [self refreshConnectBtns];
}

/**
 ****************************************************************************************
 * @brief       delegate ble update connected peripheral.
 *
 * @param[out]  aPeripheral : the connected peripheral.
 *
 ****************************************************************************************
 */
-(void)bleDidRetrievePeripheral : (NSArray *)aPeripheral{
    QppP2CVCState = QPP_P2C_RETRIEVED;
}

/**
 ****************************************************************************************
 * @brief       delegate ble update connected peripheral.
 *
 * @param[out]  aPeripheral : the connected peripheral.
 *
 ****************************************************************************************
 */
-(void)bleDidFailToConnectPeripheral : (CBPeripheral *)aPeripheral
                               error : (NSError *)error{
    QppP2CVCState = QPP_P2C_IDLE;
}

#pragma mark - the delegate from QPP layer

/**
 ****************************************************************************************
 * @brief       Qpp receive data delegate .
 *
 * @param[out]  aPeripheral          : the connected peripheral.
 * @param[out]  qppUUIDForNotifyChar : the UUID for Notify Char.
 * @param[out]  qppData              : the data received.
 *
 ****************************************************************************************
 */
- (void)didQppReceiveData : (CBPeripheral *) aPeripheral
             withCharUUID : (CBUUID *)qppUUIDForNotifyChar
                 withData : (NSData *)qppData
{
    const int8_t *rspData = [qppData bytes];
    
    if(rspData == nil)
    {
        return;
    }
    
    [_qppReceivedDataFromChar setText : [NSString stringWithFormat:@"%@", [self CBUUIDToUUID : qppUUIDForNotifyChar]]];
    
    [_qppReceivedData setText : [NSString stringWithFormat:@"%@", qppData]];
    
    int8_t curData = rspData[0];
    
    int8_t diff = curData - preData;
    
    if(diff != 1)
    {
        [_qppPtErrorData setText : [NSString stringWithFormat:@"%@", qppData]];
        
        ptErrorCounter++;
        
        [_qppPtErrorCounter setText:[NSString stringWithFormat:@"%d", ptErrorCounter]];
    }
    
    preData = rspData[0];
    
    if(flagDataMonitoring)
    {
        if(dataRateStart == rspData[0])
        {
            [[NSNotificationCenter defaultCenter] postNotificationName : strQppUpdateDataRateNoti object:nil ];
        }
    }
    else{
        flagDataMonitoring = TRUE;
        
        if(dataRateStart != rspData[0])
        {
            dataRateStart = rspData[0];
        }
    }
}

/**
 ****************************************************************************************
 * @brief       app scan peripheral around.
 *
 * @param[in]  sender : id
 *
 ****************************************************************************************
 */
- (IBAction)ScanPeri:(id)sender {
    qBleClient *dev = [qBleClient sharedInstance];
    
    [self qppUserConfig];
    
    BOOL isConnected = qppConnectedPeri.isConnected;
    
    if (isConnected) {
        [self qppReset];
        
        QppP2CVCState = QPP_P2C_DISCONNECTING;
        
        [dev pubDisconnectPeripheral : qppConnectedPeri];
    }
    else
    {
        if((QppP2CVCState != QPP_P2C_SCANNING) &&
           (QppP2CVCState != QPP_P2C_CONNECTING) )
        {
            QppP2CVCState = QPP_P2C_SCANNING;
            
            [self qppStartScanActInd];
            
            _ptScanCountDnLbl.text = [NSString stringWithFormat:@"%d", QPP_SCAN_DEV_TIMEOUT];
            
            [dev stopScan];
            
            [dev startScan];
        }
    }
}

/**
 ****************************************************************************************
 * @brief app refresh scan button.
 ****************************************************************************************
 */
- (void)refreshConnectBtns { 
    BOOL isConnected = qppConnectedPeri.isConnected; 
    
    if (isConnected) {
        [self.connectButton setTitle:@"Disconnect" forState:UIControlStateNormal];
        
        NSString *dev_name = qppConnectedPeri.name;          
        if (dev_name) {
            self.devNameLabel.text = dev_name; 
        }
        
        self.connStatusLabel.text = @"<>";
    }
    else {
        [self.connectButton setTitle:@"Scan" forState:UIControlStateNormal];
        self.connStatusLabel.text = @"><";
    }
}

#pragma mark - RECEIVED_DATA_LENGTH
/**
 ****************************************************************************************
 * @brief app Discovered Services Response.
 ****************************************************************************************
 */
- (void)qppDidDiscoveredServicesRsp{
    NSLog(@"%s", __func__);
    
}

/**
 ****************************************************************************************
 * @brief  app Discovered Services Char.
 ****************************************************************************************
 */
- (void)qppDidDiscoveredCharsRsp{
    NSLog(@"%s", __func__); 
}

/// update data
/**
 ****************************************************************************************
 * @brief update state for notify char response.
 ****************************************************************************************
 */
- (void)qppUpdateStateForCharRsp:(NSNotification *)noti{
    NSLog(@"%s", __func__);
}

/**
 ****************************************************************************************
 * @brief Select one peripheral response.
 ****************************************************************************************
 */
- (void)qppSelOnePeripheralRsp :(NSNotification *)noti
{
    // CBPeripheral *selectedPeri
    qppConnectedPeri = [noti.userInfo objectForKey: keyQppSelectPeri];
        
    /// to conect the peripheral
    qBleClient *dev = [qBleClient sharedInstance];
    [dev stopScan];
    
    QppP2CVCState = QPP_P2C_CONNECTING;
    
    [self qppStartDidConnActInd];
    
    [dev pubConnectPeripheral : qppConnectedPeri];
}

/**
 ****************************************************************************************
 * @brief Qpp get a file response.
 ****************************************************************************************
 */
- (void)qppGetFileDataRsp : (NSNotification *)noti{
    fileData = [noti.userInfo objectForKey: keyQppFileData];
    
    fileDataBuf = (int8_t *)[fileData bytes];
    fileLength = [fileData length];
}

/**
 ****************************************************************************************
 * @brief Qpp Sendout Dtat on time response.
 ****************************************************************************************
 */
- (void)qppDidSendDataOnTimeRsp
{
    QppApi *qppApi = [QppApi sharedInstance];
    
    uint8_t i;
    
    uint16_t remainDiff = fileLength - fileDataIdx;
    
    if(remainDiff > (qppC2pPkgNum * QPP_PKG_UNIT_LENGTH))
    {
        for (i = 0; i < qppC2pPkgNum; i++)
        {
            memcpy(sendData, &fileDataBuf[fileDataIdx], QPP_PKG_UNIT_LENGTH);
        
            NSData *c2pData = [[NSData alloc] initWithBytes:sendData length : QPP_PKG_UNIT_LENGTH];
        
            [qppApi qppSendData : qppConnectedPeri
                   withData : c2pData];
        
            fileDataIdx += QPP_PKG_UNIT_LENGTH;
        }
    }
    else{
        uint8_t remainDiffPkg = remainDiff / QPP_PKG_UNIT_LENGTH;
        
        for (i = 0; i < remainDiffPkg; i++)
        {
            memcpy(sendData, &fileDataBuf[fileDataIdx], QPP_PKG_UNIT_LENGTH);
            
            NSData *c2pData = [[NSData alloc] initWithBytes : sendData length : QPP_PKG_UNIT_LENGTH];
            
            [qppApi qppSendData : qppConnectedPeri
                       withData : c2pData];
            
            fileDataIdx += QPP_PKG_UNIT_LENGTH;
        }
        
        /// send the less than QPP_PKG_UNIT_LENGTH
        uint8_t diff = (uint8_t)(fileLength - fileDataIdx);
        
        if(diff)
        {
            memcpy(sendData, &fileDataBuf[fileDataIdx], diff);
        
            NSData *c2pData = [[NSData alloc] initWithBytes:sendData length : diff];
        
            [qppApi qppSendData : qppConnectedPeri
                       withData : c2pData];
        }
        
        /// stop timer to send.
        [qppDidSendOnTimeTimer invalidate];
        
        [[NSNotificationCenter defaultCenter] postNotificationName : strQppSendFileEndNoti object:nil];
        
        NSLog(@"Send stop!");
    }
    
//    NSLog(@"next interval\n");
}

/**
 ****************************************************************************************
 * @brief Qpp Sendout Data on time response.
 * @para [in] : onOff        : start/stop
 * @para [in] : loadInterval : load data interval
 * @para [in] : pkgNum       : package num each interval.
                               max data 20bytes per package
 ****************************************************************************************
 */
- (void)qppLoadStart : (BOOL)onOff
           withIntvl : (uint8_t)loadInterval
           withPkgNum:(uint8_t)pkgNum
{    
    qppC2pPkgNum = pkgNum;
    fileDataIdx = 0;
    
    if(onOff)
    {
        qppDidSendOnTimeTimer = [NSTimer scheduledTimerWithTimeInterval : loadInterval * 0.01f target:self selector:@selector(qppDidSendDataOnTimeRsp) userInfo:nil repeats : YES];
    }
    else{
        [qppDidSendOnTimeTimer invalidate];
        
//        NSLog(@"Send stop.");
    }
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation{
    
    return YES;
}

/**
 ****************************************************************************************
 * @brief Qpp count down scan time out.
 * @para [in] : flag        : display or not
 * @para [in] : ScanCountDn : scan time count down
 ****************************************************************************************
 */
- (void)updateScanCountDown : (BOOL)flag
                  withCount : (uint8_t)ScanCountDn
{
    if(flag)
    {
        _ptScanCountDnLbl.text = [NSString stringWithFormat:@"%d", ScanCountDn];
        _ptScanCountDnLbl.hidden = NO;
        _ptScanCountDnUnitLbl.hidden = NO;
    }
    else
    {
        _ptScanCountDnLbl.text = @"0";
        _ptScanCountDnLbl.hidden = YES;
        _ptScanCountDnUnitLbl.hidden = YES;
    }
}

/**
 ****************************************************************************************
 * @brief Qpp Stop scan device time out.
 ****************************************************************************************
 */
- (void) qppStopScanDevTimeout
{
    QppP2CVCState = QPP_P2C_SCANNED;
    
    qppScanDevTimeoutCount = 0;
    
    [qppScanDevTimeoutTimer invalidate];
    
    [self.ptScanDevActInd stopAnimating];
}

/**
 ****************************************************************************************
 * @brief Qpp Scan device time out response.
 ****************************************************************************************
 */
- (void) pbScanDevTimeoutRsp{
    if(qppScanDevTimeoutCount < QPP_SCAN_DEV_TIMEOUT)
    {
        qppScanDevTimeoutCount++;
        
        [self updateScanCountDown : TRUE withCount : (QPP_SCAN_DEV_TIMEOUT - qppScanDevTimeoutCount)];
        
        return;
    }
    
    [self updateScanCountDown : FALSE withCount : 0];
    
    [self qppStopScanDevTimeout];
    
    qBleClient *dev = [qBleClient sharedInstance];
    
    // create the alert
    NSArray *pbDevList = [dev discoveredPeripherals];
    
    if([pbDevList count])
    {
        [[NSNotificationCenter defaultCenter] postNotificationName : strQppScanPeriEndNoti object:nil];
    }
    else
    {
        /// to scan a device timeout.
        CustomAlertView *pbScanDevAlert = [[CustomAlertView alloc] initWithTitle : ALERT_NODEVICE_TITLE
                                                                         message : @"No device found!"
                                                                        delegate : nil
                                                               cancelButtonTitle : nil/*@"Cancel" */
                                                               otherButtonTitles : @"OK", nil];
        [pbScanDevAlert show];
    }
}

/**
 ****************************************************************************************
 * @brief Qpp Start Scan device Activity Indicator.
 ****************************************************************************************
 */
- (void) qppStartScanActInd
{
    [self.ptScanDevActInd startAnimating];
    
    qppScanDevTimeoutCount = 0;
    
    qppScanDevTimeoutTimer = [NSTimer scheduledTimerWithTimeInterval : 1.0 target:self selector:@selector(pbScanDevTimeoutRsp) userInfo:nil repeats : YES];
}
 
/**
 ****************************************************************************************
 * @brief Qpp display peripherals scanned.
 ****************************************************************************************
 */
- (void)qppDisplayPeripherals 
{
    [self.ptScanDevActInd stopAnimating];
    
    [[NSNotificationCenter defaultCenter] postNotificationName: ReloadDevListDataNoti object:nil userInfo:nil];
    
    [self presentModalViewController : deviceVC animated:YES];
}

/**
 ****************************************************************************************
 * @brief Qpp Stop connect device timeout.
 ****************************************************************************************
 */
- (void) qppStopDidConnDevTimeout
{
    qppDidConnDevTimeoutCount = 0;
    [qppDidConnDevTimeoutTimer invalidate];
    [self.ptDidConnDevActInd stopAnimating];
}

/**
 ****************************************************************************************
 * @brief Qpp Connect device timeout response.
 ****************************************************************************************
 */
- (void) qppDidConnDevTimeoutRsp{
    // if(qppDidConnDevTimeoutCount > QPP_DIDCONN_DEV_TIMEOUT)
    {
        [self qppStopDidConnDevTimeout];
        
        /// to scan a device timeout.
        CustomAlertView *pbDidConnDevAlert = [[CustomAlertView alloc] initWithTitle : ALERT_CONNECT_FAIL_TITLE
                                                                            message:@"Connection failed!"
                                                                           delegate:nil
                                                                  cancelButtonTitle:nil /*@"Cancel" */
                                                                  otherButtonTitles:@"OK", nil];
        [pbDidConnDevAlert show];
    }
}

/**
 ****************************************************************************************
 * @brief Qpp Start connect device Activity Indicator.
 ****************************************************************************************
 */
- (void) qppStartDidConnActInd
{
    [self.ptDidConnDevActInd startAnimating];
    
    qppDidConnDevTimeoutTimer = [NSTimer scheduledTimerWithTimeInterval : QPP_DIDCONN_DEV_TIMEOUT target:self selector:@selector(qppDidConnDevTimeoutRsp) userInfo:nil repeats : NO];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

///
/**
 ****************************************************************************************
 * @brief Qpp Get reference time.
 ****************************************************************************************
 */
- (NSDate *)getDateTimeFromMilliSeconds:(uint64_t) miliSeconds
{
    NSTimeInterval tempMilli = miliSeconds;
    NSTimeInterval seconds = tempMilli/1000.0;
 
    return [NSDate dateWithTimeIntervalSince1970:seconds];
}

/**
 ****************************************************************************************
 * @brief Qpp Start Scan device Activity Indicator.
 * convert time with NSDate format into NSInteger, from 1970/1/1
 ****************************************************************************************
 */
- (uint64_t)getDateTimeTOMilliSeconds:(NSDate *)datetime
{
    NSTimeInterval interval = [datetime timeIntervalSince1970];
     
    uint64_t totalMilliseconds = interval*1000 ; 
    
    return totalMilliseconds;
}

/**
 ****************************************************************************************
 * @brief Qpp Update Data Rate.
 ****************************************************************************************
 */
- (void)qppUpdateDataRate{
    NSDate *  currentTime = [NSDate date];
    
    NSDateFormatter  *dateformatter=[[NSDateFormatter alloc] init];
    
    [dateformatter setDateFormat:@"HH:mm:ss.SSSS"];
    
    curTimeMs = [self getDateTimeTOMilliSeconds : currentTime];
    
    uint64_t deltaTime = curTimeMs - preTimeMs;
    
    
    float qppCurDataRate = (255 * 20 * 1000 / deltaTime);
    
    preTimeMs = curTimeMs;
    
    _qppDataRateCurLbl.text = [NSString stringWithFormat:@"%d", (int)qppCurDataRate];
    
    if(qppCurDataRate/* /1000 */ <= 0)
        return;
    
    // min max
    if(qppDataRateMin > (qppCurDataRate ))
    {
        qppDataRateMin  = qppCurDataRate ;
    }
    // max
    if(qppDataRateMax < qppCurDataRate)
    {
        qppDataRateMax  = qppCurDataRate;
    }
}

/**
 ****************************************************************************************
 * @brief Qpp convert CBUUID to UUID.
 ****************************************************************************************
 */
-(NSString *) CBUUIDToUUID : (CBUUID *) UUID {
    
    NSString *strUUID = [NSString stringWithFormat:@"%s",[[UUID.data description] cStringUsingEncoding : NSStringEncodingConversionAllowLossy]];
    
    return strUUID;
}
@end
