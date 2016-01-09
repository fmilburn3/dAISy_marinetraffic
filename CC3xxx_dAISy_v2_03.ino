/*
  Receives data from dAISy and forwards it to MarineTraffic using a TI CC3xxx LP / BP and Energia
  and Energia.  Tested on the following....

  dAISy            CC3xxx              Energia         Comments
  --------------   -----------------   -------------   -------------------------------------------
  F5529/BackPack   CC3200              v17 non-EMT     Not yet tested but expected to work
  F5529/BackPack   TM4C123 w/ CC3100   v17             Stable
  F5529/BackPack   F5529 w/ CC3100     v17             Stable when the the ring buffer is increased
                                                       ring buffer is increased in HardwareSerial.cpp
                                                       e.g. #define SERIAL_BUFFER_SIZE 512 
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
  
  This code is released in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
        
  Created by Frank Milburn, January 2015
  @gmtii at 43oh.com published a sketch for the ESP8266 from which this is derived
  Thanks for assistance from @chicken and @spirillis at 43oh.com
*/

#ifndef __CC3200R1M1RGC__
#include <SPI.h>                               // do not include SPI for CC3200 LaunchPad
#endif

#include <WiFi.h>
#include "accounts.h"

#define MAXLEN 100                             // maximum allowed length of a NMEA sentence
#define DEBUG 0                                // make DEBUG 0 to turn off extra print

char ssid[]     = WIFI_SSID;                   // WIFI_SSID and WIFI_PASSWORD are defined         
char password[] = WIFI_PASSWORD;               // in accounts.h

char serverIP[] = SERVER_IP;                   // SERVER_IP and SERVER_PORT are defined             
int  serverPort = SERVER_PORT;                 // in accounts.h

char nmea[MAXLEN];                             // holds incoming NMEA sentences   
int blinkSentence = 0;                         // toggles RED_LED when sentences are sent
                                               // to MarineTraffic
WiFiClient client;

void setup()
{
  pinMode(RED_LED, OUTPUT);
  Serial1.begin(38400);                        // dAISy transmits to LP/CC3xxx at 38400 baud
                                               // on Serial1  
  Serial.begin(115200);                        // output from LP/CC3xxx to serial monitor is
                                               // at 115200 baud on Serial
  delay(500);                                  // let Serial catch up...
  Serial.println("Starting CC3xxx_dAISy_v2_20\n");
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

  Serial.print("\nMarineTraffic connect: ");   // Connect to marinetraffic   
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

  while (!client.connected()) {               // disconnected from MarineTraffic
    client.flush();                            
    client.stop();
    delay(5000);
    Serial.println("Not connected to MarineTraffic - attempt to connect");
    client.connect(serverIP, serverPort);
    delay(5000);
  }
   
  static int i = 0;                            // the counter i keeps track of characters read into the
                                               // nmea array.  It is set to zero the first time it is
                                               // encountered but retains it's value from then on.
  while (Serial1.available()) {                // read incoming NMEA sentences from dAISy       
    if (i < MAXLEN-1) {
      nmea[i] = (char)Serial1.read();          // place new chars into nmea[]
    }
    nmea[++i] = 0;                             // increment the counter and store a end of string marker                                                     
    if (nmea[i-1] == '\n') {                   // newline indicates the end of the NMEA sentence
      if (DEBUG) Serial.print(nmea);           
      if (blinkSentence == 0) {
        blinkSentence = 1;
      }
      else {
        blinkSentence = 0;
      }
      digitalWrite(RED_LED, blinkSentence);    // toggle the red LED for each sentence sent to MarineTraffic
      client.print(nmea);                      // send the sentence to MarineTraffic
      i = 0;                                   // now set the counter back to 0
      nmea[i] = '\0';                          // and initialize the first element of nema to null          
    }                                           
  }
}

void printWifiStatus() {
  Serial.print("SSID: ");                      // print SSID of the network
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();               // print WiFi shield's IP address:
  Serial.print("IP Address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();                     // print received signal strength:
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
