# dAISy_marinetraffic
This Energia sketch uses dAISy and the Texas Instruments CC3200 LaunchPad or CC3100 BoosterPack to create a low cost shore based AIS station that reports data to MarineTraffic.

Tested on the following with Energia v17....

  dAISy: F5529/BackPack
  
  TM4C123 w/ CC3100
  
  F5529 w/ CC3100 - must increase the ring buffer in HardwareSerial.cpp of Energia - see comments in code
  
  CC3200 - not tested with this version of the code but expected to work

Released into the public domain

Created by Frank Milburn, January 2015

  @gmtii at 43oh.com published a sketch for the ESP8266 from the starting framework of this code was derived
  
  Thanks for assistance from @chicken and @spirillis at 43oh.com

See the following link for more information on dAISy: https://www.tindie.com/products/astuder/daisy-ais-receiver/

For more information on MarineTraffic and setting up a receiving station: http://www.marinetraffic.com/

For information on the Texas Instruments CC3200-LAUNCHXL: http://www.ti.com/ww/en/launchpad/launchpads-connected-cc3200-launchxl.html

For information on the Texas Instruments CC3100BOOST:
http://www.ti.com/tool/cc3100boost?intc=searchrecs-mcu&keyMatch=cc3100boost
