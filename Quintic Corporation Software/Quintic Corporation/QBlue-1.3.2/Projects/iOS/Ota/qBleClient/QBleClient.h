//
//  QBleClient.h
//  Qpp Demo
//
//  @brief Application Programming Interface Header File for Quintic Ble Client.
//
//  Created by quintic on 5/18/14.
//  Copyright (c) 2014 Quintic. All rights reserved.
//

#import <Foundation/Foundation.h>

#if TARGET_OS_IPHONE
#import <CoreBluetooth/CoreBluetooth.h>
#else
#import <IOBluetooth/IOBluetooth.h>
#endif

///// to notify discovered Services to app layer
#define bleDiscoveredServicesNoti                  @"ble-DiscoverServicesNoti"

/// to notify discovered Services to app layer
#define bleDiscoveredCharacteristicsNoti            @"ble-DiscoveredCharacteristicsNoti"

@protocol bleDidConnectionsDelegate

/**
 ****************************************************************************************
 * @brief       delegate ble update connected peripheral.
 *
 * @param[out]  aPeripheral : the connected peripheral.
 *
 ****************************************************************************************
 */
-(void)bleDidConnectPeripheral : (CBPeripheral *)aPeripheral;

/**
 ****************************************************************************************
 * @brief       delegate ble update disconnected peripheral.
 *
 * @param[out]  aPeripheral : the disconnected peripheral.
 * @param[out]  error
 *
 ****************************************************************************************
 */
-(void)bleDidDisconnectPeripheral : (CBPeripheral *)aPeripheral error : (NSError *)error;

/**
 ****************************************************************************************
 * @brief       delegate ble update connected peripheral.
 *
 * @param[out]  aPeripheral : the connected peripheral.
 *
 ****************************************************************************************
 */
-(void)bleDidRetrievePeripheral : (NSArray *)aPeripheral;

/**
 ****************************************************************************************
 * @brief       delegate ble update connected peripheral.
 *
 * @param[out]  aPeripheral : the connected peripheral.
 *
 ****************************************************************************************
 */
-(void)bleDidFailToConnectPeripheral : (CBPeripheral *)aPeripheral
                               error : (NSError *)error;

@end

/// ble update for char delegate.
@protocol bleUpdateForOtaDelegate

/**
 ****************************************************************************************
 * @brief       delegate ble update service and chara.
 *
 * @param[out]  aPeripheral    : the peripheral connected.
 * @param[out]  aService       : the OTA service discovered.
 * @param[out]  error          : the error from CoreBluetooth if there is.
 *
 ****************************************************************************************
 */
-(void)bleDidUpdateCharForOtaService : (CBPeripheral *)aPeri
                      withService : (CBService *)aService
                            error : (NSError *)error;

/**
 ****************************************************************************************
 * @brief       delegate ble update value for Char.
 *
 * @param[out]  aService       : the OTA service discovered.
 * @param[out]  characteristic : the OTA characteristic updated.
 * @param[out]  error          : the error from CoreBluetooth if there is.
 *
 ****************************************************************************************
 */
-(void)bleDidUpdateValueForOtaChar : (CBService *)aService
                       withChar : (CBCharacteristic *)characteristic
                          error : (NSError *)error;

@end

/// declare qBleClient class
@class qBleClient;

/// declare qBleClient interface
@interface qBleClient : NSObject <CBCentralManagerDelegate, CBPeripheralDelegate>{
    
}

/// update Connection/Disconnect/Retrieve/FailtoConnect Peripheral.
@property (nonatomic, assign) id <bleDidConnectionsDelegate> bleDidConnectionsDelegate;

/// update value/state for char to xxx Profile
@property (nonatomic, assign) id <bleUpdateForOtaDelegate>  bleUpdateForOtaDelegate;

/**********************************************************************
 * @brief output these discovered peripherals after scanning.
 *
 **********************************************************************/
@property (nonatomic, readonly, retain) NSMutableArray *discoveredPeripherals;

/**********************************************************************
 * @brief output these discovered peripherals after scanning.
 *
 **********************************************************************/
@property (nonatomic, readonly, retain) NSMutableArray *discoveredServices;

/**
 **********************************************************************
 * @brief Represents the current state of a CBCentralManager.
 *
 * @return : TRUE  (CBCentralManagerStatePoweredOn)
 *           FALSE (others).
 *
 * @ref    enum CBCentralManagerState
 **********************************************************************
 */
- (BOOL) isLECapableHardware;


/**
 **********************************************************************
 * @brief scan all ble peripherals.
 *
 **********************************************************************
 */
- (void) startScan;

/**
 **********************************************************************
 * @brief stop scan.
 *
 **********************************************************************
 */
- (void) stopScan;

/**
 **********************************************************************
 * @brief Api Or App layer to connect a peripheral
 *
 * @param[in] aPeripheral  : the peripheral to connnect.
 *
 **********************************************************************
 */
- (void) pubConnectPeripheral:(CBPeripheral *)aPeripheral;

/**
 **********************************************************************
 * @brief Api Or App layer to disconnect a peripheral
 *
 * @param[in] aPeripheral  : the peripheral to disconnect.
 *
 **********************************************************************
 */
- (void) pubDisconnectPeripheral:(CBPeripheral *)aPeripheral;

/**
 **********************************************************************
 * @brief Api Or App layer to retrieve a Peripheral
 *
 * @param[in] aPeripheral  : the peripheral to retrieve
 *
 **********************************************************************
 */
- (void) pubRetrievePeripheral:(CBPeripheral *)aPeripheral;

+ (qBleClient *)sharedInstance;

@end

