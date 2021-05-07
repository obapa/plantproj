#include "wifiSetupOb.h"

#ifndef SLEEPTIME
#define SLEEPTIME (uint32_t)10e6
#endif

#ifndef STASSID
#define STASSID "paternal"
#define STAPSK  "al7rog52hd"

const char wSsid[] = STASSID;
const char wPass[] = STAPSK;
#endif



espWifi::espWifi(uint8_t* _staticIP, uint8_t* _gateway, uint8_t* _subnet) : staticIP(_staticIP), gateway(_gateway), subnet(_subnet) {
	wifiOff();
};


void espWifi::wifiOff(){
	//discconect and put into modem sleep
	WiFi.disconnect();
	WiFi.mode( WIFI_OFF );
	WiFi.forceSleepBegin();
	delay(1);
}

//function to enable wifi,
//if a previous session is found in the rtc, will attempt to use that
void espWifi::wifiOn(){
	WiFi.forceSleepWake();
	delay(1);
	WiFi.persistent( false );
	WiFi.mode( WIFI_STA );
	if( isRtcValid() ){
		WiFi.begin( wSsid, wPass, rtcData.channel, rtcData.bssid, true );
	}else{
		WiFi.begin( wSsid, wPass );
	}
	//WiFi.config( staticIP, gateway, subnet );
}

//function to verify WiFi connection
//if unable to connect to WiFi after 30 seconds, will set device to deepsleep
void espWifi::wifiConnect(){
	int retries = 0;
	int wifiStatus = WiFi.status();
	while( wifiStatus != WL_CONNECTED) {
		retries++;
		if( retries == 100 ){
			WiFi.disconnect();
			delay(10);
			WiFi.forceSleepBegin();
			delay(10);
			WiFi.forceSleepWake();
			delay(10);
			WiFi.begin( wSsid, wPass );
		}
		if( retries == 600 ){
			WiFi.disconnect(true);
			delay(1);
			WiFi.mode( WIFI_OFF );
			ESP.deepSleep( SLEEPTIME, WAKE_RF_DISABLED );
			return;
		}
		delay( 50 );
		wifiStatus = WiFi.status();
	}
}

//todo un-hardcode
void espWifi::writeWifiToRtc(){
	rtcData.channel = WiFi.channel();
	memcpy( rtcData.bssid, WiFi.BSSID(), 6 );
	rtcData.crc32 = calculateCRC32( ((uint8_t*)&rtcData) + 4, sizeof( rtcData ) - 4 );
	ESP.rtcUserMemoryWrite( 0, (uint32_t*)&rtcData, sizeof( rtcData ) );
}

//function for calculating the CRC32 checksum
uint32_t espWifi::calculateCRC32( const uint8_t *data, size_t length){
	uint32_t crc = 0xffffffff;
	while( length-- ){
		uint8_t c = *data++;
		for( uint32_t i = 0x80; i > 0; i >>= 1) {
			bool bit = crc & 0x80000000;
			if( c & i ){
				bit = !bit;
			}

			crc <<= 1;
			if( bit ){
				crc ^= 0x04c11db7;
			}
		}
	}

	return crc;
}

//function to verify rtc checksum
bool espWifi::isRtcValid(){
	if( ESP.rtcUserMemoryRead( 0, (uint32_t*)&rtcData, sizeof( rtcData ) ) ){
		uint32_t crc = calculateCRC32( ((uint8_t*)&rtcData) + 4, sizeof( rtcData ) - 4 );
		return (bool)( crc == rtcData.crc32 );
	}
	return false;
}
