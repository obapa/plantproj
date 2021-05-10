#ifndef rtcHelper
#define rtcHelper

#ifndef WiFi_h
	#include <ESP8266WiFi.h>
#endif
#ifndef wificlientbearssl_h
	#include <WiFiClientSecure.h>
#endif



class RTCMem
{
public:
	//ESP8266 RTC memory is 128 x 32 bit
	//0-12 bytes are used for storring wifi information
	//13-98 are for storring ssl session
	//ESP8266 RTC memory is 128 x 32 bit
	//The following data structure utilizes 12 bytes
	struct {
		uint32_t crc32;   // 4 bytes
		uint8_t channel;  // 1 byte
		uint8_t bssid[6]; // 6 bytes
		uint8_t padding;  // 1 byte
	} NetworkConfigRtcData;

	struct{
		uint32_t crc32; //4 bytes
		uint8_t session_id[32];
		uint8_t session_id_len;
		uint16_t version;
		uint16_t cipher_suite;
		uint8_t master_secret[48];
	} TlsSessionRtcData;

	RTCMem();

	void saveSession(BearSSL::Session sesh);

	uint32_t calculateCRC32( const uint8_t *data, size_t length );

	bool isRtcValid();
private:
//todo how am I going to structure the memory and save everything to it
	//todo sizeof for determining where in the memory you are
};


#endif

