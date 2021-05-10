#include "rtcHelper.h"

/*
//function for calculating the CRC32 checksum
uint32_t rtcHelper::calculateCRC32( const uint8_t *data, size_t length ){

}
*/
/*
bool RTCMem::isRtcValid(stringMemBlock){
	if( ESP.rtcUserMemoryRead( 0, (uint32_t*)&rtcData, sizeof( rtcData ) ) ){
		uint32_t crc = calculateCRC32( ((uint8_t*)&rtcData) + 4, sizeof( rtcData ) - 4 );
		return (bool)( crc == rtcData.crc32 );
	}
	return false;
}*/


uint32_t RTCMem::calculateCRC32( const uint8_t *data, size_t length){
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
