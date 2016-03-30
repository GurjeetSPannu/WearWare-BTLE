Quintic, Inc.
Email: Inquiry@Quinticcorp.com
Web: http://www.quinticcorp.com

QN9020 Bluetooth Low Energy Software Development Kit
Release Notes
*****************************************************************************************************************
*****************************************************************************************************************

Version 1.3.2
Jun 7, 2014

- This version supports the QN902x-B2, QN902x-B1 and QN902x-B0. The details of changes,
  enhancements and bug fixes can be found below.
  
Bug Fixes:

- There is a bug in the NVDS API 'nvds_put'. In some cases, this API will cause
  data error of NVDS area. The configuration in the NVDS may be lost.
  ('\Projects\BLE\\src\fw\nvds.h')

Known Issues:

- The DTM (Direct Test Mode) do not support data length is 0 in the QN902x-B0.
  QN902x-B1&B2 is ok.
- The stack do not process the parameter 'con_latency' in the message 
  GAP_LE_CREATE_CONN_REQ. This parameter is always zero in the air packet
  CONN_REQ. If the central role want to set slave latency, the program can use
  connection update procedure to change slave latency.
- APB clock should no more than UART clock at UART RX DMA work mode.

Version 1.3.0
May 23, 2014

Notices:

- This version supports the QN902x-B2, QN902x-B1 and QN902x-B0. The details of changes,
  enhancements and bug fixes can be found below.
  
Changes and Enhancements:

- Added examples for QPP and OTA.
  ('\Projects\BLE\src\prj_qpp')
  ('\Projects\iOS\QPP')
  ('\Projects\BLE\src\prj_ota')
  ('\Projects\iOS\OTA')
  ('\Projects\iOS\App')
- Improved ADC driver API for more convenient use.
  ('\Projects\BLE\src\driver\adc.c/h')  
  
Bug Fixes:
- Fixed a memory overflow in the anps demo project.
  ('\Projects\BLE\prj_anps\src\usr_design.c')
- Fixed a potential memory leak in the ANPC, PASPC.
  ('\Projects\BLE\src\profiles\anp\anpc\anpc_task.c')
  ('\Projects\BLE\src\profiles\pasp\paspc\paspc_task.c')

Known Issues:

- The DTM (Direct Test Mode) do not support data length is 0 in the QN902x-B0.
  QN902x-B1&B2 is ok.
- The stack do not process the parameter 'con_latency' in the message 
  GAP_LE_CREATE_CONN_REQ. This parameter is always zero in the air packet
  CONN_REQ. If the central role want to set slave latency, the program can use
  connection update procedure to change slave latency.
- APB clock should no more than UART clock at UART RX DMA work mode.


Version 1.2.0
Apr 8, 2014

Notices:

- This version supports the QN902x-B2, QN902x-B1 and QN902x-B0. The details of changes,
  enhancements and bug fixes can be found below.
  
Changes and Enhancements:

- Added examples for broadcaster role and observer role.
  ('\Projects\BLE\src\prj_broadcaster')
  ('\Projects\BLE\src\prj_observer')
- Added SPI support for EACI/ACI/HCI.
  ('usr_config.h'
   'app_config.h'
   'app_main.c'
   '\Projects\BLE\src\driver\spi.c/h'
   '\Projects\BLE\src\driver\sleep.c/h'
  )
- Added SPI TX status check API
  ('\Projects\BLE\src\driver\spi.c/h')
- Added keyword volatile to global variable which will be used in interrupt handler
  ('\Projects\BLE\src\driver\i2c.c'
   '\Projects\BLE\src\driver\adc.c/h'
  )
- Updated ADC driver comments for external reference selection, modified default configuration of ADC_SINGLE_WITHOUT_BUF_DRV mode
  ('\Projects\BLE\src\driver\adc.c/h')

Bug Fixes:

- Fixed the issue firmware version and hardware version is reversed in DIS service.
  ('\Projects\BLE\src\app\diss\anps\app_diss_task.c')

Known Issues:

- The DTM (Direct Test Mode) do not support data length is 0 in the QN902x-B0.
  QN902x-B1&B2 is ok.
- The stack do not process the parameter 'con_latency' in the message 
  GAP_LE_CREATE_CONN_REQ. This parameter is always zero in the air packet
  CONN_REQ. If the central role want to set slave latency, the program can use
  connection update procedure to change slave latency.
- APB clock should no more than UART clock at UART RX DMA work mode.


Version 1.0.0
Feb 11, 2014

Notices:

- This version supports the QN902x-B2, QN902x-B1 and QN902x-B0. The details of changes,
  enhancements and bug fixes can be found below.
  
Changes and Enhancements:

- Updated copyright information.
- Improved sleep checking flow.
  ('\Projects\BLE\src\main\app_main.c')
- Added a API for checking whether the BLE is waked up.
  ('\Projects\BLE\src\lib\lib.h')
- Modified preferred supervision timeout from 0x07d to 0x12C. Using macro
  GAP_PPCP_STO_MULT replacing 0x12C as supervision timeout.
  ('\Projects\BLE\src\fw\gap_cfg.h'
   'usr_design.c'
  )
- Improved notification sending procedure. Added Notification flow control.
  ('\Projects\BLE\src\app\')
- Deleted unnecessary power mode setting in the button callback.
  ('usr_design.c')
- Updated EACI project,added deep sleep mode.
  ('\Projects\BLE\prj_eaci_controller\src\app_eaci_generic.c')
- Added missing functions'declaration.  
  ('\Projects\BLE\src\driver\i2c.h')
- Improved parameter's type.
  ('\Projects\BLE\src\driver\pwm.c/h')
- Added 32k low power mode.
  ('\Projects\BLE\src\driver\sleep.c')
- Remove repeat RX buffer read in SPIx_RX_IRQHandler.
  ('\Projects\BLE\src\driver\spi.c')
- Improved SPI TX interrupt enable/disable function.
  ('\Projects\BLE\src\driver\spi.h')

Bug Fixes:

- The logic checking whether to enter into IDLE mode is imperfect, it is 
  possible to enter into IDLE mode at a wrong moment.
  ('\Projects\BLE\src\main\app_main.c')
- Quickly sending notifications may cause a memory leak in the ANP, CSCP, PASP
  and RSCP.
  ('\Projects\BLE\src\profiles\anp\anps\anps_task.c';
   '\Projects\BLE\src\profiles\cscp\cscps\cscps_task.c';
   '\Projects\BLE\src\profiles\pasp\pasps\pasps_task.c';
   '\Projects\BLE\src\profiles\rscp\rscps\rscps_task.c';
  )
- Connect the same device show only one of them.
  ('\Projects\BLE\prj_eaci_host\src\app\app_generic.c')
  
Known Issues:

- The DTM (Direct Test Mode) do not support data length is 0 in the QN902x-B0.
  QN902x-B1&B2 is ok.
- The stack do not process the parameter 'con_latency' in the message 
  GAP_LE_CREATE_CONN_REQ. This parameter is always zero in the air packet
  CONN_REQ. If the central role want to set slave latency, the program can use
  connection update procedure to change slave latency.
- APB clock should no more than UART clock at UART RX DMA work mode.


Version 0.9.8
JAN 09,2014

Notices:

- This version supports the QN902x-B2, QN902x-B1 and QN902x-B0. The details of changes,
  enhancements and bug fixes can be found below.

Changes and Enhancements:

- Added support of QN902x-B2.
- Created a new fold 'Driver' to store virtual COM driver and USB-UART chip driver.
- Add the assert definition to help debugging.
  ('\Projects\BLE\src\main\app_main.c';
   '\Projects\BLE\src\fw\compiler.h'
  )   
- Used chip version instead of firmware version to distinguish the chip.
  ('\Projects\BLE\src\main\app_main.c'
   '\Projects\BLE\src\lib\lib.h'
   'usr_config.h'
  )
- Used chip version instead of firmware version in the library file name.
  ('\Projects\BLE\src\lib')
- BY default used device name in the NVDS as local name in the advertising
  packet, unless the device name tag is not availiable in the NVDS, the name
  defined by macro 'CFG_LOCAL_NAME' is used in the advertising packet. 
- Added interface to support 128-bit UUID.
  ('\Projects\BLE\src\fw\atts_util.h')  
- Modified ADC driver to improve result accuracy
  ('\Projects\Driver\adc\src\adc_exampel.c')
  ('\Projects\BLE\src\driver\adc.c/h')
- Modified NVDS TAGs to be consistent with ADC improvement.
  ('\Projects\BLE\src\fw\nvds.h')   
- Modified I2C driver to support B2
  ('\Projects\BLE\src\driver\i2c.c/h')
- Added support for SPI 3-wire configuration
  ('\Projects\BLE\src\driver\spi.c/h')
- Added support for 32k clock correction with 32MHz system clock
  ('\Projects\BLE\src\driver\syscon.c')
- Rename the calibration.lib to qn902xbx_lib_lite.lib.
  ('\Projects\Driver\src\lib)

Bug Fixes:

- Used connection index instead of selected index to fix the issue of accessing
  multi-pheripherals' service uncorrectly.
  ('\Projects\BLE\src\app\anpc\app_anpc_task.c'
   '\Projects\BLE\src\app\basc\app_basc_task.c'
   '\Projects\BLE\src\app\blpc\app_blpc_task.c'
   '\Projects\BLE\src\app\cscpc\app_cscpc_task.c'
   '\Projects\BLE\src\app\disc\app_disc_task.c'
   '\Projects\BLE\src\app\findl\app_findl_task.c'
   '\Projects\BLE\src\app\hrpc\app_hrpc_task.c'
   '\Projects\BLE\src\app\htpt\app_htpc_task.c'
   '\Projects\BLE\src\app\paspc\app_paspc_task.c'
   '\Projects\BLE\src\app\proxm\app_proxm_task.c'
   '\Projects\BLE\src\app\rscpc\app_rscpc_task.c'
   '\Projects\BLE\src\app\scppc\app_scppc_task.c'
   '\Projects\BLE\src\app\tipc\app_tipc_task.c'
  )
- Fixed the issue of Filling the parameter 'anps_ntf_alert_cmd' incompletely.
  ('\Projects\BLE\src\app\anps\app_anps.c')
- Improved to check number of connections before creating connection.
  ('\Projects\BLE\src\app\app_menu.c')
- Updated app_get_client_idx_by_conhdl() - added CSC and RSC profiles.
  ('\Projects\BLE\src\app\app_util.c')
- Removed auto advertising after link disconnection. 
  ('\Projects\BLE\prj_eaci_controller\src\app_eaci_generic.c')
- Update EACI's enable idx 
- Used connection index instead of selected index to fix the issue of accessing
  multi-pheripherals' service uncorrectly.
  ('\Projects\BLE\prj_eaci_controller\src\app_eaci_an.c'
   '\Projects\BLE\prj_eaci_controller\src\app_eaci_batt.c'
   '\Projects\BLE\prj_eaci_controller\src\app_eaci_blp.c'
   '\Projects\BLE\prj_eaci_controller\src\app_eaci_csc.c'
   '\Projects\BLE\prj_eaci_controller\src\app_eaci_dis.c'
   '\Projects\BLE\prj_eaci_controller\src\app_eaci_fm.c'
   '\Projects\BLE\prj_eaci_controller\src\app_eaci_gl.c'
   '\Projects\BLE\prj_eaci_controller\src\app_eaci_hrp.c'
   '\Projects\BLE\prj_eaci_controller\src\app_eaci_ht.c'
   '\Projects\BLE\prj_eaci_controller\src\app_eaci_pas.c'
   '\Projects\BLE\prj_eaci_controller\src\app_eaci_prx.c'
   '\Projects\BLE\prj_eaci_controller\src\app_eaci_rsc.c'
   '\Projects\BLE\prj_eaci_controller\src\app_eaci_sp.c'
   '\Projects\BLE\prj_eaci_controller\src\app_eaci_time.c'
  ) 
- Modified EACI to check number of connections before calling app_gap_le_create_conn_req().
  ('\Projects\BLE\prj_eaci_controller\src\app_eaci_generic.c')
  
- Modified ANALOG driver for lower sleep leakage current
  ('\Projects\BLE\src\driver\analog.c')
- Improved RF frequency sweep example, remove calibration test
  ('\Projects\Driver\analog\src\analog_example.c')
  
- Fixed a serial flash API bug
  ('\Projects\BLE\src\driver\serialflash.c/h')
- Switch off REF PLL power before enter sleep for B1
  ('\Projects\BLE\src\driver\sleep.c/h')

Known Issues:

- The DTM (Direct Test Mode) do not support data length is 0 in the QN902x-B0.
  QN902x-B1&B2 is ok.
- The stack do not process the parameter 'con_latency' in the message 
  GAP_LE_CREATE_CONN_REQ. This parameter is always zero in the air packet
  CONN_REQ. If the central role want to set slave latency, the program can use
  connection update procedure to change slave latency.
- APB clock should no more than UART clock at UART RX DMA work mode.


Version 0.9.6
Nov 27, 2013

Notices:

- This version supports the QN902x-B1 and QN902x-B0. The details of changes,
  enhancements and bug fixes can be found below.
  
Changes and Enhancements:

- Added support of QN902x-B1.
- Added a general interface 'QBlueStudio' to manage all of the tools, 
  documents and example code. 
  ('\Tools\QBlue\QBlue_v1.00.exe')
- Removed reg_ble_sleep_cb() invocation.
  ('\Projects\BLE\src\main\app_main.c')
- Moved 'save_ble_setting()' into the function enter_sleep().
  ('\Projects\BLE\src\main\app_main.c')
- Added one parameter 'clk_32k' in the plf_init() to indicate which 32k clock is
  used in the system.
  ('\Projects\BLE\src\lib\lib.h'; '\Projects\BLE\src\main\app_main.c')
- Added interface to set BLE program latency.
  ('\Projects\BLE\src\lib\lib.h')
- Added API sleep_post_process() to finish sleep post process.  
  ('\Projects\BLE\src\lib\lib.h')
- Added API set_dbg_info() to record debug information in the memory.
  ('\Projects\BLE\src\lib\lib.h')  
- Added messages to support service change.
  ('\Projects\BLE\src\fw\gatt_task.h')
- Added interface to set accurate timer.
  ('\Projects\BLE\src\fw\ke_timer.h')
- Added two NVDS TAGs to support temperature sensor and ADC.
  ('\Projects\BLE\src\fw\nvds.h')  
- Added sanity check. 
  ('\Projects\BLE\src\app\gap\app_gap_task.c')
- Added debug information. 
  ('\Projects\BLE\src\app\paspc\app_paspc_task.c')
- Cleaned debug information. 
  ('\Projects\BLE\src\app\app_menu.c')
- Improved EACI.
  ('\Projects\BLE\prj_eaci_controller')
- Removed cb_before_ble_enter_sleep() and cb_after_ble_exit_sleep().
  ('\Projects\BLE\prj_xxx\src\usr_design.c/h')  
- Improved link script.
  ('\Projects\BLE\src\link')  
- Corrected comments. 
  ('\Projects\BLE\src\cmsis\driver_QN9020.h')
- Added new register defination for QN902x-B1. 
  ('\Projects\BLE\src\cmsis\QN9020.h')
- Modified power mode macro definition.
  PM_ACTIVE instead of CO_PD_DISALLOWED
  PM_IDLE instead of CPU_CLK_OFF_ALLOW
  PM_SLEEP instead of CPU_POWER_DOWN_ALLOW
  PM_DEEP_SLEEP instead of CPU_DEEP_SLEEP_ALLO
  ('\Projects\BLE\src\driver\sleep.h')
- Modified sleep procedure for QN902x-B1.
  ('\Projects\BLE\src\driver\sleep.c/h')
- Improved debug information in ADC example.
  ('\Projects\Driver\adc\src\adc_exampel.c')
- Removed unnecessary procedure in analog example.
  ('\Projects\Driver\adc\src\analog_exampel.c')
- Improved ADC driver, added input buffer gain setting API. 
  ('\Projects\BLE\src\driver\adc.c/h')
- Improved I2C driver, added APIs of I2C multi-bytes access, updated I2C clock calculate formula. 
  ('\Projects\BLE\src\driver\i2c.c/h')
- Added an interface to dump registers using UART.
- Adjusted the sequence of UART baudrate setting.
- Added UART TX/RX DMA operation option
  ('\Projects\BLE\src\driver\uart.c/h')
- Added SPI TX/RX DMA operation option
  ('\Projects\BLE\src\driver\spi.c/h')
- Improved PWM driver for sleep check
  ('\Projects\BLE\src\driver\pwm.c/h')
- Added AHB/APB clock get API, added a global variable g_Apbclock.
  ('\Projects\BLE\src\driver\syscon.c/h')
- Improved DMA driver: don't set DMA active bit at UART RX DMA mode(allow sleep)
  ('\Projects\BLE\src\driver\dma.c')
- Removed state check in GLPC abort operation.
  ('\Projects\BLE\src\app\app_menu.c')
- Adjusted operation sequence in ANPC menu and changed a parameter.
  ('\Projects\BLE\src\app\app_menu.c')
- Removed function app_get_client_idx_by_conhdl().
  ('\Projects\BLE\src\app\app_util.c')
  ('\Projects\BLE\src\app\app_glpc_task.c')
  ('\Projects\BLE\src\app\app_hrpc_task.c')
  ('\Projects\BLE\src\app\app_proxm_task.c')

Bug Fixes:

- Corrected the reason of local disconnection request.
  ('\Projects\BLE\src\app\gap\app_gap.c')
- Corrected the ringer control point's requirement flag.
  ('\Projects\BLE\src\profiles\pasp\paspc\paspc_task.c')
- Update eaci_controller's data
  ('\Projects\BLE\prj_eaci_controller\src\app_eaci_blp.c'
   '\Projects\BLE\prj_eaci_controller\src\app_eaci_dis.c'
   '\Projects\BLE\prj_eaci_controller\src\app_eaci_gl.c'
   '\Projects\BLE\prj_eaci_controller\src\app_eaci_hrp.c'
   '\Projects\BLE\prj_eaci_controller\src\app_eaci_time.c'
   '\Projects\BLE\prj_eaci_controller\src\app_eaci_fm.c'
   '\Projects\BLE\prj_eaci_controller\src\app_eaci_csc.c'
   '\Projects\BLE\prj_eaci_controller\src\app_eaci_ht.c'
   '\Projects\BLE\prj_eaci_controller\src\app_eaci_sp.c'
  )
- Fixed connection parameters updated failed issue.

Known Issues:

- The DTM (Direct Test Mode) do not support data length is 0 in the QN902x-B0.
  QN902x-B1 is ok.
- The stack do not process the parameter 'con_latency' in the message 
  GAP_LE_CREATE_CONN_REQ. This parameter is always zero in the air packet
  CONN_REQ. If the central role want to set slave latency, the program can use
  connection update procedure to change slave latency.
- APB clock should no more than UART clock at UART RX DMA work mode


Version 0.9.4
Oct 9, 2013

Notices:

- This version only supports the QN902x-B0. The details of changes,
  enhancements and bug fixes can be found below.

Changes and Enhancements:

- Added five new profiles. (\BLE\src\profiles;\BLE\src\app)
    Alert Notification Profile
    Phone Alert Status Profile
    Cycling Speed and Candence Profile
    Running Speed and Candence Profile
    HID over GATT Profile
- Support timer profile multi-instance.
- Check peripherals' sleep status before BLE's sleep status. 
  (\BLE\src\main\app_main.c)
- Added 32k RCO support.
- Added sleep control APIs in the sleep.c/h, and moved function usr_sleep() from
  usr_design.c to sleep.c.
- Set device sleep status in the driver.
- Prevent sleep to wait for 32k XTAL ready after waking up from deep sleep.
- Optimized device driver configuration for code size down.
- Added three APIs to control serial flash.
- Used one GPIO interrupt callback instead of independent callback for code size down.

Bug Fixes:
- Removed duplicated function declaration. (app_gap_task.h app_gap_adv_intv_update_timer_handler())
- Corrected key distribution setting. (app_gap.c)

Known Issues:

- The DTM (Direct Test Mode) do not support data length is 0.
- The stack do not process the parameter 'con_latency' in the message 
  GAP_LE_CREATE_CONN_REQ. This parameter is always zero in the air packet
  CONN_REQ. If the central role want to set slave latency, the program can use
  connection update procedure to change slave latency.

Version 0.9.2
Aug 28, 2013

Notices:

- This version only supports the QN902x-B0. The details of changes,
  enhancements and bug fixes can be found below.

Changes and Enhancements:

- Reduced default stack size.
- Added constraint in the link script.
- Adjusted optimization to level 3.
- Changed default AHB clock.
- Improved analog control.
- Removed unused led API.
- Improved memory retention configuration interface.
- Added deep sleep support.
- Improved system clock setting API for code size reduction.
- Improved the drivers of timer, SPI, RTC, PWM, DMA, ADC for code size reduction.
- Added configured ability of HCI/ACI uart interface.
- Added GATT write response interface for QTool.
- Added kernel time API to get BLE time counter.
- Added API to set the ppm of 32k clock.
- Merged sleep and deep sleep for code size reduction.
- Provided an alternative way to reduce sleep current.
- Improved customization of application API.
- Added simple peripheral example for current testing.
- Health thermometer service example has been updated to use ADC and temperature
  sensor to capture real temperature.
- Distinguished libraray file based on GAP roles.
- Adjusted oscillator warm up duration in the NVDS. The configuration in the
  NVDS shall be reprogramed.

Bug Fixes:

- Corrected the issue in the IO configuration of driver example.  
- Due to the timing issue the advertising will be stopped abnormally
  in very few cases. Add a watchdog to deal with this issue.
  
Known Issues:

- The DTM (Direct Test Mode) do not support data length is 0.
- The stack do not process the parameter 'con_latency' in the message 
  GAP_LE_CREATE_CONN_REQ. This parameter is always zero in the air packet
  CONN_REQ. If the central role want to set slave latency, the program can use
  connection update procedure to change slave latency.
  

Version 0.9.0
July 15, 2013

Notices:

- This version of the Quintic BLE software release only supports 
  the QN902x-B0. QN902x-A1 is no longer supported since changes
  in the hardware.
  The details of changes, enhancements and bug fixes can be found
  below.
  
Changes and Enhancements:

- There is a change in the pin definition. Related software is
  revised especially the gpio driver. The software tool 'QnDriverTools'
  is updated to support new pin definition.
- Added IAR IDE support for all projects. The software tool
  'QnDevDBforKeil' is updated to support both Keil and IAR.
- Event functionality is added to provide deferral capacity of some
  procedure.
- Multi-instance profile support when the device is client role.
- Two GATT messages are added to control resource access.
- Added four new HCI vendor commands to support accessing register
  and NVDS.
- Battery service example has been updated to use ADC to capture
  real voltage.
- Moved the clock setting macro to the file 'driver_config.h' in order to
  gather user configuration items.

Bug Fixes:
  
- Sleep current has a huge decrease since the hardware bug fixed.
- Improved stability when using sleep mode.

Known Issues:

- The DTM (Direct Test Mode) do not support data length is 0.


Version 0.8.2
May 31, 2013

Notices:

- The Quintic Bluetooth low energy (BLE) software development kit 
  includes all necessary software and documents to get started on
  the development of single-mode BLE applications using the QN9020
  system-on-chip. 

- The following are bin files:

    <Install Directory>\BinFiles\bass.bin

    <Install Directory>\BinFiles\blps.bin

    <Install Directory>\BinFiles\client.bin

    <Install Directory>\BinFiles\controller.bin

    <Install Directory>\BinFiles\eaci_controller.bin

    <Install Directory>\BinFiles\eaci_host.bin

    <Install Directory>\BinFiles\eapi.bin

    <Install Directory>\BinFiles\np_controller.bin

    <Install Directory>\BinFiles\findt.bin

    <Install Directory>\BinFiles\glps.bin

    <Install Directory>\BinFiles\hrps.bin

    <Install Directory>\BinFiles\htpt.bin

    <Install Directory>\BinFiles\proxr.bin

    <Install Directory>\BinFiles\scpps.bin

    <Install Directory>\BinFiles\tips.bin

 
- The following additional documentation are included:

    
    <Install Directory>\Documents\HardWare\BLE_IFA_Antenna_Design_AN v0.1.pdf

    <Install Directory>\Documents\HardWare\QBLUE_DK_Motherboard_User_Guide v0.3.pdf

    <Install Directory>\Documents\HardWare\QBLUE_DK_QN902x_EVB_User_Guide v0.2.pdf

    <Install Directory>\Documents\HardWare\QBLUE_DK_Quick_Start v0.4.pdf

    <Install Directory>\Documents\HardWare\QN902x_Hardware_Application_Note v0.2.pdf

    <Install Directory>\Documents\Software\Driver Tools Manual v0.3.pdf

    <Install Directory>\Documents\Software\ISP Studio Manual v0.6.pdf

    <Install Directory>\Documents\Software\Keil Device Database Manual v0.3.pdf

    <Install Directory>\Documents\Software\NVDS Configurator Manual v0.4.pdf

    <Install Directory>\Documents\Software\QN9020 API Programming Guide v0.3.pdf

    <Install Directory>\Documents\Software\QN9020 Easy ACI Programming Guide v0.2.pdf

    <Install Directory>\Documents\Software\QN9020 Easy API Programming Guide v0.1.pdf

    <Install Directory>\Documents\Software\QN9020 Software Developer's Guide v0.5.pdf

    <Install Directory>\Documents\Software\QTool User Manual v0.3.pdf


- The following Windows PC application are included:


    QnDevDBforKeil:
    <Install Directory>\Tools\QnDevDBforKeil\QnDevDBforKeil v1.06.exe

    QnDriverTools:
    <Install Directory>\Tools\QnDriverTools\QnDriverTools v1.17.exe

    QnNVDSConfigurator:
    <Install Directory>\Tools\QnNVDSConfigurator\QnNVDSConfigurator v1.19.exe

    QTool:
    <Install Directory>\Tools\QTool\QTool v1.0.4.exe

    QnISPStudio:
    <Install Directory>\Tools\QnISPStudio\QnISPStudio v1.17.exe


- The following software projects are included, and built by Keil:

  BLE:
    Battery:
    <Install Directory>\Projects\BLE\prj_bass

    Blood Pressure
    <Install Directory>\Projects\BLE\prj_blps

    GATT client role:
    <Install Directory>\Projects\BLE\prj_client

    Controller mode:
    <Install Directory>\Projects\BLE\prj_controller_mode

    Easy ACI Controller:
    <Install Directory>\Projects\BLE\prj_eaci_controller

    Easy ACI Controller:
    <Install Directory>\Projects\BLE\prj_eaci_host

    Easy API:
    <Install Directory>\Projects\BLE\prj_eapi

    Find Me:
    <Install Directory>\Projects\BLE\prj_findt

    Glucose:
    <Install Directory>\Projects\BLE\prj_glps

    Heart Rate:
    <Install Directory>\Projects\BLE\prj_hrps

    Health Thermometer:
    <Install Directory>\Projects\BLE\prj_htpt

    Proximity:
    <Install Directory>\Projects\BLE\prj_proxr

    Scan:
    <Install Directory>\Projects\BLE\prj_scpps

    Time:
    <Install Directory>\Projects\BLE\prj_tips

  Driver:
    ADC:
    <Install Directory>\Projects\driver\Keil\adc\adc.uvproj

    Analog:
    <Install Directory>\Projects\driver\Keil\analog\analog.uvproj

    DMA:
    <Install Directory>\Projects\driver\Keil\dma\dma.uvproj

    GPIO:
    <Install Directory>\Projects\driver\Keil\gpio\gpio.uvproj

    I2C:
    <Install Directory>\Projects\driver\Keil\i2c\i2c.uvproj

    PWM:
    <Install Directory>\Projects\driver\Keil\pwm\pwm.uvproj

    RTC:
    <Install Directory>\Projects\driver\Keil\rtc\rtc.uvproj

    SerialFlash:
    <Install Directory>\Projects\driver\Keil\serialflash\serialflash.uvproj

    Sleep:
    <Install Directory>\Projects\driver\Keil\sleep\sleep.uvproj

    SPI:
    <Install Directory>\Projects\driver\Keil\spi\spi.uvproj

    Timer:
    <Install Directory>\Projects\driver\Keil\timer\timer.uvproj

    UART:
    <Install Directory>\Projects\driver\Keil\uart\uart.uvproj

    WDT:
    <Install Directory>\Projects\driver\Keil\wdt\wdt.uvproj

Changes:
 
- Initial Release

Bug Fixes:
 
- Initial Release


*****************************************************************************************************************
*****************************************************************************************************************
