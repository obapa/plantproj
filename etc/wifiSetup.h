#ifndef WiFi_h
	#include <ESP8266WiFi.h>
#endif
#ifndef wificlientbearssl_h
	#include <WiFiClientSecure.h>
#endif


//function for calculating the CRC32 checksum
uint32_t calculateCRC32( const uint8_t *data, size_t length);


void wifiOff();
void wifiOn();

//function to verify WiFi connection
//if unable to connect to WiFi after 30 seconds, will set device to deepsleep
void wifiConnect();

//function to verify rtc checksum
bool isRtcValid();

//function to save WiFi session to rtc
void writeWifiToRtc();

