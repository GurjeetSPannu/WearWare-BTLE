//
//  QBleClient.m
//  Qpp Demo
//
// @brief Application Programming Interface Source File for Quintic Ble Client.
//
//  Created by quintic on 5/18/14.
//  Copyright (c) 2014 Quintic. All rights reserved.
//

#import "QBleClient.h"

@interface qBleClient ()
{
    CBCentralManager *_centralManager;
    CBPeripheral     *_peripheral;

    NSMutableArray   *_discoveredPeripherals;
    NSMutableArray   *_discoveredServices;

    BOOL _autoConnect;
}
@end

@implementation qBleClient

@synthesize bleDidConnectionsDelegate;
@synthesize bleUpdateForOtaDelegate;
@synthesize discoveredPeripherals = _discoveredPeripherals;
@synthesize discoveredServices = _discoveredServices;

- (id)init
{
    self = [super init];
    
    if (self) {
        _centralManager = [[CBCentralManager alloc]initWithDelegate:self
                                                              queue:nil];

        _discoveredPeripherals = [[NSMutableArray alloc] init];
        _discoveredServices = [[NSMutableArray alloc] init];
        
        if (_autoConnect) {
            [self startScan];
        }
    }
    
    return self;
}

- (void)dealloc
{
    [self stopScan];
    _centralManager.delegate = nil;
    _peripheral.delegate = nil;
}

+ (qBleClient *)sharedInstance
{
    static qBleClient *_sharedInstance = nil;
    if (_sharedInstance == nil) {
        _sharedInstance = [[qBleClient alloc] init];
    }
    
    return _sharedInstance;
}

/**********************************************************************
 Uses CBCentralManager to check whether the current platform/hardware supports Bluetooth LE.
 An alert is raised if Bluetooth LE is not enabled or is not supported.
 
 CBCentralManagerStateUnknown = 0,
 CBCentralManagerStateResetting,
 CBCentralManagerStateUnsupported,
 CBCentralManagerStateUnauthorized,
 CBCentralManagerStatePoweredOff,
 CBCentralManagerStatePoweredOn ONLY maps to TRUE
 
 **********************************************************************/
- (BOOL) isLECapableHardware
{
    switch ([_centralManager state])
    {
        case CBCentralManagerStateUnsupported:
            NSLog(@"The platform/hardware doesn't support Bluetooth Low Energy.");
            break;
        case CBCentralManagerStateUnauthorized:
            NSLog(@"The app is not authorized to use Bluetooth Low Energy.");
            break;
        case CBCentralManagerStatePoweredOff:
            NSLog(@"Bluetooth is currently powered off.");
            break;
        case CBCentralManagerStatePoweredOn:
            NSLog(@"isLECapableHardware: TRUE");
            return TRUE;
        case CBCentralManagerStateUnknown:
        default:
            return FALSE;
            
    }
    
    return FALSE;
}

/*********************************************************************
 start Scan Peripherals
 In : None
 Out : None
 Return : None
 *********************************************************************/
- (void)startScan
{
    [_discoveredPeripherals removeAllObjects];
    [_centralManager scanForPeripheralsWithServices:nil 
                                            options:nil];
}

/*********************************************************************
 stop Scan Peripherals
 In : None
 Out : None
 Return : None
 *********************************************************************/
- (void)stopScan
{
    [_centralManager stopScan];
}

/*********************************************************************
 connect a Peripheral
 In :
 aPeripheral  : to connect
 Out : None
 
 Return : None
 *********************************************************************/
- (void)pubConnectPeripheral:(CBPeripheral *)aPeripheral
{
    [_centralManager connectPeripheral:aPeripheral options:nil];
    _peripheral = aPeripheral;
}

/*********************************************************************
 disconnect a Peripheral
 In :
 aPeripheral  : to disconnect
 Out : None
 
 Return : None
 *********************************************************************/
- (void)pubDisconnectPeripheral:(CBPeripheral *)aPeripheral
{
    [_centralManager cancelPeripheralConnection : aPeripheral ];
}

/*********************************************************************
 retrieve a Peripheral
 In :
 aPeripheral  : to retrieve
 Out : None
 
 Return : None
 *********************************************************************/
-(void)pubRetrievePeripheral : (CBPeripheral *)aPeripheral
{
    [_centralManager retrievePeripherals : [NSArray arrayWithObject:(id)aPeripheral.UUID]];
}

#pragma mark - CBCentralManagerDelegate

/*********************************************************************
 Invoked whenever the central manager's state is updated.
 *********************************************************************/
- (void)centralManagerDidUpdateState:(CBCentralManager *)central
{
    [self isLECapableHardware];
}

/******************************************************************
 Invoked when the central discovers Qpp peripheral while scanning.
 ******************************************************************/
- (void) centralManager:(CBCentralManager *)central
  didDiscoverPeripheral:(CBPeripheral *)aPeripheral
      advertisementData:(NSDictionary *)advertisementData
                   RSSI:(NSNumber *)RSSI
{
    if( ![_discoveredPeripherals containsObject:aPeripheral] )
    {
        [_discoveredPeripherals addObject:aPeripheral];
    }
    
    /* Retreive already known devices */
    if(_autoConnect)
    {
        [_centralManager retrievePeripherals:[NSArray arrayWithObject:(id)aPeripheral.UUID]];
    }
}

/*********************************************************************
 Invoked when the central manager retrieves the list of known peripherals.
 Automatically connect to first known peripheral
 *********************************************************************/
- (void)centralManager:(CBCentralManager *)central
    didRetrievePeripherals : (NSArray *)peripherals
{
    [self stopScan];
    
    /* If there are any known devices, automatically connect to it.*/
    if([peripherals count] > 0)
    {
        _peripheral = [peripherals objectAtIndex : 0];
        
        [_centralManager connectPeripheral:_peripheral
                                   options:[NSDictionary dictionaryWithObject:[NSNumber numberWithBool:YES] forKey:CBConnectPeripheralOptionNotifyOnDisconnectionKey]
         ];
    }
}

/*********************************************************************
 Invoked whenever a connection is succesfully created with the peripheral.
 Discover available services on the peripheral
 *********************************************************************/
- (void) centralManager : (CBCentralManager *)central
   didConnectPeripheral : (CBPeripheral *)aPeripheral
{
    [aPeripheral setDelegate : self];
    [aPeripheral discoverServices : nil];
	
    [bleDidConnectionsDelegate bleDidConnectPeripheral : aPeripheral];
}

/*********************************************************************
 Invoked whenever an existing connection with the peripheral is torn down.
 Reset local variables
 *********************************************************************/
- (void)centralManager      : (CBCentralManager *)central
    didDisconnectPeripheral : (CBPeripheral *)aPeripheral
                      error : (NSError *)error
{
    if( aPeripheral)
    {
        [bleDidConnectionsDelegate bleDidDisconnectPeripheral : aPeripheral error :error];
        
        aPeripheral.delegate = nil;
        aPeripheral = nil;
    }
}

/*********************************************************************
 Invoked whenever the central manager fails to create a connection 
 with the peripheral.
 *********************************************************************/
- (void)centralManager : (CBCentralManager *)central
didFailToConnectPeripheral : (CBPeripheral *)aPeripheral
                 error : (NSError *)error
{
    if( aPeripheral )
    {
        [bleDidConnectionsDelegate bleDidFailToConnectPeripheral : aPeripheral error :error];
        
        [aPeripheral setDelegate:nil];
        aPeripheral = nil;
    }
}

/*********************************************************************
 Invoked whenever the central manager retrieve to create a connection 
 with the peripheral.
 *********************************************************************/
- (void)centralManager:(CBCentralManager *)central didRetrieveConnectedPeripherals:(NSArray *)peripherals
{
    [bleDidConnectionsDelegate bleDidRetrievePeripheral : peripherals];
}

#pragma mark - CBPeripheralDelegate
/*********************************************************************
 Invoked upon completion of a -[discoverServices:] request.
 Discover available characteristics on interested services
 *********************************************************************/
- (void) peripheral:(CBPeripheral *)aPeripheral
    didDiscoverServices:(NSError *)error
{
    [_discoveredServices removeAllObjects];
    
    for (CBService *aService in aPeripheral.services)
    {
        [aPeripheral discoverCharacteristics : nil forService : aService];
        
        if( ![_discoveredServices containsObject : aService] )
        {
            [_discoveredServices addObject : aService];
        }
    }
    
    [[NSNotificationCenter defaultCenter] postNotificationName : bleDiscoveredServicesNoti
                                                        object : nil
                                                      userInfo : nil];
}

/*********************************************************************
 Invoked upon completion of a -[discoverCharacteristics:forService:] request.
 Perform appropriate operations on interested characteristics
 *********************************************************************/
- (void) peripheral : (CBPeripheral *)aPeripheral
    didDiscoverCharacteristicsForService : (CBService *)service error : (NSError *)error
{
    /// for quintic profile delegate
    [bleUpdateForOtaDelegate bleDidUpdateCharForOtaService : aPeripheral
                                               withService : service
                                                     error : error];
    
    [[NSNotificationCenter defaultCenter] postNotificationName : bleDiscoveredCharacteristicsNoti
                                                        object : nil
                                                      userInfo : nil];
}

/*********************************************************************
 Invoked upon completion of a -[updateValueForCharacteristic:] 
 request or on the reception of a notification/indication.
 *********************************************************************/
- (void) peripheral:(CBPeripheral *)aPeripheral didUpdateValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error
{
    for (CBService *aService in aPeripheral.services)
    {   
        [bleUpdateForOtaDelegate bleDidUpdateValueForOtaChar : aService
                                                    withChar : characteristic
                                                       error : error];
    }
}

/*********************************************************************
 Invoked upon completion of a -[UpdateNotificationStateForCharacteristic:]
 request or on the reception of a notification/indication.
 *********************************************************************/
- (void)peripheral:(CBPeripheral *)peripheral didUpdateNotificationStateForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error
{

}
@end
