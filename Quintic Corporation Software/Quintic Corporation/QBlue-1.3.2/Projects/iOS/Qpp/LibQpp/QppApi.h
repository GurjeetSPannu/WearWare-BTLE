//
//  QppApi.h
//  Qpp Demo
//
//  @brief Application Programming Interface Header File for Quintic Private Profile.
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

#import "QBleClient.h"

/// QPP receive data delegate.
@protocol qppReceiveDataDelegate

/**
 ****************************************************************************************
 * @brief APP receive data from QppApi Layer.
 *
 * @param[out]  aPeripheral          : the Peripheral connected. 
 * @param[out]  qppUUIDForNotifyChar : the Characteristic Notified.
 * @param[out]  qppData              : the data received.
 *
 ****************************************************************************************
 */
-(void)didQppReceiveData : (CBPeripheral *)aPeripheral
            withCharUUID : (CBUUID *)qppUUIDForNotifyChar
                withData : (NSData *)qppData;  

@end

/// an Objective-C interface 
@interface QppApi : NSObject<bleUpdateForQppDelegate>{
    
}

@property (nonatomic, assign) id <qppReceiveDataDelegate> ptReceiveDataDelegate;

/**
 ****************************************************************************************
 * @brief app register QPP's peripheral UUID and service UUID to Api layer.
 *
 * @param[in]  qppServiceUUID : the service UUID to discover.
 * @param[in]  writeCharUUID  : the Characteristic UUID to write.
 *
 * @return none
 ****************************************************************************************
 */
- (void) qppRegUUIDs : (NSString *)qppServiceUUID
          withWrChar : (NSString *)writeCharUUID;

/**
 ****************************************************************************************
 * @brief transfer data to QPP
 *
 * @param[in]  aPeripheral : the Peripheral connected.
 * @param[in]  qppData     : the data sent.
 *
 * @return none
 ****************************************************************************************
 */
-(void)qppSendData : (CBPeripheral *)aPeripheral withData : (NSData*)qppData;

/**
 ****************************************************************************************
 * @brief QppApi class method.
 *
 * @param[out]  all methods.
 *
 * @return : none
 ****************************************************************************************
 */
+ (QppApi *)sharedInstance;

@end
