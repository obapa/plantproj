#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <time.h>
#include "etc/extraFunctions.h"
#include <avr/pgmspace.h>

//#include "etc/wifiSetup.h"
#include "etc/wifiSetupOb.h"
//#include "etc/rtcHelper.h"

/*
#ifndef _SYS__STDINT_H
#include <_stdint.h>
#endif
*/
//ADC_MODE(ADC_VCC)//todo to read VCC
//call ESP.getVcc()

//I believe I am getting a progmem error when this is saved there. How much flash memory do I have available?
static const char root_ca[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIF1TCCBL2gAwIBAgISBJYVJTBpjUWkcHabOl2aI/xUMA0GCSqGSIb3DQEBCwUA
MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD
EwJSMzAeFw0yMTAzMTYwNzE4MjRaFw0yMTA2MTQwNzE4MjRaMBcxFTATBgNVBAMT
DHBvYmFuaW9uLmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANfI
OTlcoxdkS9jtU2KYx1c8Uv/P+DqfvUjGqwIpYqZ4IuXeGQHK/edMLrsOMa/R0p2m
BfuW3ZnU7pRfrAGBY5G3VY6TcZ3s45H4BnZ2CbrBLpXp3BX/eXprYNZGw8KNKbZi
Z+eHkuO03yDPnBdLjoot1gE+XYYJVfHC3M3BovRrCayLvSW3JjizbRzt/qDRh3di
jdbn+hz+5lV6NZTd8kUH16KBxW1lNB7oJ5qkfSSoqYoNZWr/5Krs3StLdL2TzWEX
s4Fn0SXqA4xE4oYt5uNxbimPh/Q40OGE6G6WT8S8vFQ7cxtn5zRvuBSrKXnmHC3+
FCDA/OscxohnlOgZKWUCAwEAAaOCAv4wggL6MA4GA1UdDwEB/wQEAwIFoDAdBgNV
HSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwDAYDVR0TAQH/BAIwADAdBgNVHQ4E
FgQUk9DaRSNtT6a1xsUlgpzncXp7eZAwHwYDVR0jBBgwFoAUFC6zF7dYVsuuUAlA
5h+vnYsUwsYwVQYIKwYBBQUHAQEESTBHMCEGCCsGAQUFBzABhhVodHRwOi8vcjMu
by5sZW5jci5vcmcwIgYIKwYBBQUHMAKGFmh0dHA6Ly9yMy5pLmxlbmNyLm9yZy8w
gc0GA1UdEQSBxTCBwoIZYXV0b2Rpc2NvdmVyLnBvYmFuaW9uLmNvbYITY3BhbmVs
LnBvYmFuaW9uLmNvbYIYY3BjYWxlbmRhcnMucG9iYW5pb24uY29tghdjcGNvbnRh
Y3RzLnBvYmFuaW9uLmNvbYIRbWFpbC5wb2Jhbmlvbi5jb22CDHBvYmFuaW9uLmNv
bYIUd2ViZGlzay5wb2Jhbmlvbi5jb22CFHdlYm1haWwucG9iYW5pb24uY29tghB3
d3cucG9iYW5pb24uY29tMEwGA1UdIARFMEMwCAYGZ4EMAQIBMDcGCysGAQQBgt8T
AQEBMCgwJgYIKwYBBQUHAgEWGmh0dHA6Ly9jcHMubGV0c2VuY3J5cHQub3JnMIIB
BAYKKwYBBAHWeQIEAgSB9QSB8gDwAHYARJRlLrDuzq/EQAfYqP4owNrmgr7YyzG1
P9MzlrW2gagAAAF4Oh1X6wAABAMARzBFAiEAiALy1fHBVKq+c5kjKmGjIm1GVuPu
JJgxojOVwFwfvUoCIHbliFMfNLaoFsC7wr0KXH1SQhwPTG84tSblbakr0R4MAHYA
9lyUL9F3MCIUVBgIMJRWjuNNExkzv98MLyALzE7xZOMAAAF4Oh1X4AAABAMARzBF
AiEAlpjMQ3YEfh3yHW8F1wMwxuiH9qxY8TMvRAWzrKCewEQCIAR5CO5Tm8ZTmwcj
/AzxnrrCmcpToK6TG83qo/JBt1CxMA0GCSqGSIb3DQEBCwUAA4IBAQCRbAtQbpLM
oS/XrZ0DuQBqlwaP3lB9e0R1RQwzG90XXEqaQRjhj9jZQN/i6LSrCXbZOnaiYcZb
sJQb4SRYUZPTEX2WBqcCs7WUnaxp4B9g9h+cD6aH978utTAuIZhUGP7gm1Gpeont
b2LCDLtndBDf7YZqRbtW5rBAEXGI+07WX8T9nvJQ4QUuN+0eoqmIWfraw6J0hrYk
J01s+AseW/cZMozcjiabLypVej6hYvRaDFJSQ0pHdiMupYmqTvL3ZYOINOROXfMg
I0YYyxcCdSND/VH8T0K/Lp9Kw74gjF25EUoxglWSlBnuWggiyYhuKZUc4jUFYnte
QORkgjEakd8o
-----END CERTIFICATE-----
)EOF";

#ifndef SLEEPTIME
#define SLEEPTIME (uint32_t)10e6
#endif



//const int httpsPort[] PROGMEM = {443};

//const char fHost[] PROGMEM = "www.pobanion.com";//instead of pulling from progmem could use macro: f("string")
const char host[] = "www.pobanion.com";
const char url[] = "/api/plant/addtodb";
const uint16_t port = 443;
//const char fFingerprint[] PROGMEM = "A7:72:52:B4:7A:AA:65:A7:49:B2:56:30:01:9C:9D:9A:8B:9C:AF:3C";

const bool debug = true;
//TODO can add wifimanager library to handle initial wifi config

void fetchURL(BearSSL::WiFiClientSecure *client, const char *host, const uint16_t port, const char *path);
void syncTime();

void setup(){
	//turn off wifi until we collect data
	uint8_t staticIp[] = {192,168,2,30};
	uint8_t gateway[] = {192,168,2,1};
	uint8_t subnet[] = {255,255,255,0};

	espWifi wifi(staticIp, gateway, subnet);


	if( debug ){
		Serial.begin( 74480 );
		Serial.setDebugOutput(true);
		Serial.println( "We are in" );
	}

	//Now enable wifi to send data
	wifi.wifiOn();
	wifi.wifiConnect();
	wifi.writeWifiToRtc();

	//now connected to internet

	//sync time with ntp
	syncTime();


	// Use WiFiClientSecure class to create TLS connection
	WiFiClientSecure client;
	//BearSSL::Session session;
	BearSSL::X509List cert(root_ca);
	uint32_t start, finish;


	Serial.print(F("connecting to "));
	Serial.println(host);


	Serial.print ("Initial size of session: ");
	//Serial.println(sizeof(session));
	//client.setSession(&session);
	Serial.printf("Connecting with an unitialized session...");
	start = millis();
	client.setTrustAnchors(&cert);

	Serial.print("free heap: ");
	Serial.println(ESP.getFreeHeap());
	Serial.print("frag heap: ");
	Serial.println(ESP.getHeapFragmentation());
	Serial.print("max free block heap: ");
	Serial.println(ESP.getMaxFreeBlockSize());
	Serial.println("GGGG");
	system_print_meminfo();
	Serial.println("hhhhh");
	Serial.print("loc of cert: 0x");
	Serial.println((int)&root_ca, HEX);
	Serial.print("After cert: ");

	/*
uint8_t *finalval = (uint8_t *)((int)&root_ca + sizeof(root_ca)-10);

for(int i=0;i<5;i++){
	Serial.println(finalval[i],HEX);
}*/
//TODO getting an error here due to loading from progmem
//I think the issue may be going past the available memory?
//Try using progmem to see if we can even read the cert, need to use my loading classes
//trying to save the cert not in progmem just causes it to fail
//memory locations should be fine, is it not aligned properly?

	fetchURL(&client, host, 443, url);

	finish = millis();
	Serial.printf("Total time: %dms\n", finish - start);

	/*
	//RTCMem.saveSession();
Serial.println("AAAA");
//test.saveSession(session);
unsigned char* sestest = (unsigned char*) &session;
for(unsigned int i =0;i<sizeof(session);i++){
	Serial.println(sestest[i]);
}
//Serial.println((char)session);
Serial.println("BBB");
*/
/*
	Serial.printf("Connecting with the just initialized session...");
	start = millis();
	client.setTrustAnchors(&cert);
	fetchURL(&client, host, port, url);
	finish = millis();
	Serial.printf("Total time: %dms\n", finish - start);
	Serial.print ("Final size of session: ");
	Serial.println(sizeof(session));

	ESP.rtcUserMemoryWrite( 12, (uint32_t*)&session, sizeof( session ) );
 // Serial.printf("Using fingerprint '%s'\n", fFingerprint);
  //client.setCACert(root_ca);//TODO running into issues here, not sure what I am doing with the cert
/*
	client.setFingerprint(fFingerprint);
	client.setSession();

  if (!client.connect(host, pgm_read_word(httpsPort))) {
    Serial.println(F("connection failed"));
    ESP.deepSleep(SLEEPTIME, WAKE_RF_DISABLED);
    return;
  }

  Serial.print(F("requesting URL: "));
  Serial.println(url);

char data[] =  "{\"luminosity\": \"3\", \"temperature\": \"4\"}";
/*char wifiPackage[] = "String("POST ") + rfFlash(fUrl) + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "content-type: application/json\r\n" +
               "Content-Length: " + strlen(data) + "\r\n" +
               "Accept: application/html\r\n" +
               "user: pobo\r\n" +
               "pass: afsder\r\n" +
               "Connection: Close\r\n\r\n" "*/

/*
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "content-type: application/json\r\n" +
               "Content-Length: " + strlen(data) +"\r\n" +
               "Accept: application/html\r\n" +
               "user: pobo\r\n" +
               "pass: afsder\r\n" +
               "Connection: Close\r\n\r\n" +
               data + "\r\n");

  Serial.println(F("request sent"));
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
    if (line == "\r") {
      Serial.println(F("headers received"));
      //Serial.println(line);
      break;
    }
  }
  String line = client.readStringUntil('\n');
Serial.println(client.readString());//Mess up whenever the page errors. Look like there is an with the page redirects.
/*  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println(client.readString());
  Serial.println("==========");
  Serial.println("closing connection");
  Serial.println(client);*/

 // wifiOff();
  //ESP.deepSleep( SLEEPTIME, WAKE_RF_DISABLED);
}

void syncTime(){
	// Set up time to allow for certificate validation
	configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
//ntp timesync for tls session
	Serial.print("Waiting for NTP time sync: ");
	time_t now = time(nullptr);
	while (now < 8 * 3600 * 2) {
		delay(500);
		Serial.print(".");
		now = time(nullptr);
	}
	Serial.println("");
	struct tm timeinfo;
	gmtime_r(&now, &timeinfo);
	Serial.print("Current time: ");
	Serial.println(asctime(&timeinfo));
}

// Try and connect using a WiFiClientBearSSL to specified host:port and dump HTTP response
void fetchURL(BearSSL::WiFiClientSecure *client, const char *host, const uint16_t port, const char *path) {
  if (!path) {
    path = "/";
  }

  Serial.printf("Trying: %s:443...", host);
  client->connect(host, port);
  Serial.println(F("got here"));
  if (!client->connected()) {
    Serial.printf("*** Can't connect. ***\n-------\n");
    return;
  }
  Serial.printf("Connected!\n-------\n");
  client->write("GET ");
  client->write(path);
  client->write(" HTTP/1.0\r\nHost: ");
  client->write(host);
  client->write("\r\nUser-Agent: ESP8266\r\n");
  client->write("\r\n");
  uint32_t to = millis() + 5000;
  if (client->connected()) {
    do {
      char tmp[32];
      memset(tmp, 0, 32);
      int rlen = client->read((uint8_t*)tmp, sizeof(tmp) - 1);
      yield();
      if (rlen < 0) {
        break;
      }
      // Only print out first line up to \r, then abort connection
      char *nl = strchr(tmp, '\r');
      if (nl) {
        *nl = 0;
        Serial.print(tmp);
        break;
      }
      Serial.print(tmp);
    } while (millis() < to);
  }
  client->stop();
  Serial.printf("\n-------\n\n");
}

void loop() {
  //readTemp(A0);
  //Serial.println("test");
  delay(1000);
}
