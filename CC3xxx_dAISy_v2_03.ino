/*
  Receives data from dAISy and forwards it to MarineTraffic using a TI CC3xxx LP / BP and Energia
  and Energia.

  dAISy            CC3xxx              Energia         Status
  --------------   -----------------   -------------   -------------------------------------------
  F5529/BackPack   CC3200              v17 non-EMT     Not tested
  F5529/BackPack   TM4C123 w/ CC3100   v17             Unstable - "random" NMEA sentence errors
  F5529/BackPack   F5529 w/ CC3100     v17             Not tested
  
  Notes:
  1) You must enable the auxiliary serial output from the dAISy debug menu for this sketch
     to receive data.  Enter "2" to connect at 38400 baud.  Make sure that debug is off.  
     Then save the settings.
  2) Enter you own wifi and MarineTraffic account data in accounts.h
  
  Connections: 
  F5529/dAISy BP          CC3200                  LaunchPad w/ CC3100
  --------------          --------------          -------------------
  P4.4 (TX)               Pin 10 RX(1)            Pin 3 RX(1)
  GND                     GND                     GND
        
  Created by Frank Milburn, December 2015
*/

#ifndef __CC3200R1M1RGC__
#include <SPI.h>                               // Do not include SPI for CC3200 LaunchPad
#endif

#include <WiFi.h>
#include "accounts.h"

char ssid[]     = WIFI_SSID;                   // WIFI_SSID and WIFI_PASSWORD are defined         
char password[] = WIFI_PASSWORD;               // in accounts.h

char serverIP[] = SERVER_IP;                   // SERVER_IP and SERVER_PORT are defined             
int  serverPort = SERVER_PORT;                 // in accounts.h

String inputData = "";                         // holds incoming data

WiFiClient client;

void setup()
{
  Serial1.begin(38400);                        // dAISy transmits to CC3200 at 38400 baud
                                               // on Serial1  
  Serial.begin(115200);                        // Output from CC3200 to serial monitor is
                                               // at 115200 baud on Serial
  delay(500);                                  // Let serial catch up...
  Serial.println("Starting CC3xxx_dAISy_v2_03\n");
  Serial.print("Network connection: ");
  WiFi.begin(ssid, password);                  // Connect to network
  while ( WiFi.status() != WL_CONNECTED) {
    delay(300);
  }  
  Serial.println("Connected ");
  while (WiFi.localIP() == INADDR_NONE) {
    delay(300);
  }
  printWifiStatus();

  Serial.print("\nMarineTraffic connect: ");     
  if (client.connect(serverIP, serverPort)) {
    Serial.println("Connected");
    Serial.print("IP: ");
    Serial.print(serverIP);
    Serial.print(", port ");
    Serial.println(serverPort);
    Serial.println("");
  }
}

void loop() {

  while (!client.connected()) {                 // disconnected from MarineTraffic
    client.flush();                            
    client.stop();
    delay(5000);
    Serial.println("Not connected to MarineTraffic - attempt to connect");
    client.connect(serverIP, serverPort);
    delay(5000);
  }
  char inChar;
  while (Serial1.available()) {                 // look for data from dAISy

    inChar = (char)Serial1.read();
    inputData += inChar;
        
    if (inChar == '\n') {
      //Serial.print(String(inputData));
      client.print(String(inputData));         // send the string to MarineTraffic
      inputData = "";                          // clear the string:
  }
  }
}

void printWifiStatus() {
  Serial.print("SSID: ");                      // print the SSID of the network you're attached to:
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();               // print your WiFi shield's IP address:
  Serial.print("IP Address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();                     // print the received signal strength:
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}




