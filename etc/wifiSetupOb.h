#ifndef WiFi_h
	#include <ESP8266WiFi.h>
#endif
#ifndef wificlientbearssl_h
	#include <WiFiClientSecure.h>
#endif

#ifndef wifiSetupOb_h
#define wifiSetupOb_h


class espWifi
{
	IPAddress staticIP;
	IPAddress gateway;
	IPAddress subnet;

	//ESP8266 RTC memory is 128 x 32 bit
	//The following data structure utilizes 12 bytes
	struct {
	  uint32_t crc32;   // 4 bytes
	  uint8_t channel;  // 1 byte
	  uint8_t bssid[6]; // 6 bytes
	  uint8_t padding;  // 1 byte
	} rtcData;
public:
	espWifi(uint8_t* _staticIP, uint8_t* _gateway, uint8_t* _subnet);

	void wifiOff();

	void wifiOn();

	void wifiConnect();

	void writeWifiToRtc();

	uint32_t calculateCRC32( const uint8_t *data, size_t length);

	bool isRtcValid();

};

#endif
