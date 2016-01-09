# dAISy_marinetraffic
This Energia sketch uses dAISy and the Texas Instruments CC3200 LaunchPad or CC3100 BoosterPack to create a low cost shore based AIS station that reports data to MarineTraffic.

Tested on the following....

  dAISy             CC3xxx                Energia           Comments
  --------------    -----------------     -------------     -------------------------------------------
  
  F5529/BackPack    CC3200                v17 non-EMT       Not yet tested but expected to work
  
  F5529/BackPack    TM4C123 w/ CC3100     v17               Stable
  
  F5529/BackPack    F5529 w/ CC3100       v17               Stable when the the ring buffer is increased
  
                                                            ring buffer is increased in HardwareSerial.cpp
                                                            e.g. #define SERIAL_BUFFER_SIZE 512 

Created by Frank Milburn, January 2015

See the following link for more information on dAISy: https://www.tindie.com/products/astuder/daisy-ais-receiver/

For more information on MarineTraffic and setting up a receiving station: http://www.marinetraffic.com/

For information on the Texas Instruments CC3200-LAUNCHXL: http://www.ti.com/ww/en/launchpad/launchpads-connected-cc3200-launchxl.html

For information on the Texas Instruments CC3100BOOST:
http://www.ti.com/tool/cc3100boost?intc=searchrecs-mcu&keyMatch=cc3100boost
