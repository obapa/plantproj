#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "etc/extraFunctions.h"
#include <avr/pgmspace.h>

#include "etc/wifiSetup.h"

//ADC_MODE(ADC_VCC)//todo to read VCC
//call ESP.getVcc()


const char* root_ca= \
  "-----BEGIN CERTIFICATE REQUEST-----\n" \
  "MIIC/jCCAeYCAQAwdDEPMA0GA1UEBwwGQXVzdGluMSAwHgYJKoZIhvcNAQkBFhFw\n" \
  "b2JhbmlvbkBsaXZlLmNvbTELMAkGA1UEBhMCVVMxDjAMBgNVBAgMBVRleGFzMRUw\n" \
  "EwYDVQQDDAxwb2Jhbmlvbi5jb20xCzAJBgNVBAoMAk5BMIIBIjANBgkqhkiG9w0B\n" \
  "AQEFAAOCAQ8AMIIBCgKCAQEAz8I08QJpvyxWXB2CVypVf/ZRr/CC2LLH3Zgx5DI8\n" \
  "Wf/Qe0usPYKLrFEyig6H6GudSPY+jsuZUOR9MWjdCP8mGMw+I0r3lpDvAaOfiwjm\n" \
  "OjSwMcjbBVVflJapCe7j2/d+CwnwS0J0WqikAOU8Xfvav+qTbsAC9W3ubDWqQTub\n" \
  "ji3XXNGH2aMDgOOmVHI6XVfT4LPnp+ZHmVCW1fI2yX3t0xCEZW33I+rKAiPxitM0\n" \
  "eOwuBJ5XnecarFxfu/+s00BbiWEme7E1JOIko1JQ+2kH+zEBlT8kpCJ/3v0dutS5\n" \
  "K7NXDKpnxcUWTJkLR6LcfLn/d+rx3Tx5nbOEZOgbM95U/wIDAQABoEUwGQYJKoZI\n" \
  "hvcNAQkHMQwMCmFsN3JvZzUyaGQwKAYJKoZIhvcNAQkOMRswGTAXBgNVHREEEDAO\n" \
  "ggxwb2Jhbmlvbi5jb20wDQYJKoZIhvcNAQELBQADggEBAD5CQJMOSxHKIGQA+5n3\n" \
  "LPx5wjrJrTle19s9XVXwnTWZsFzXaWqyB3HqD+PgeQOuNQx/YV0G4Pl9tm4+jk8r\n" \
  "53CmzSjMQZqmZhGB1uj7cO5NhxuXLMldYbpLWIcA3YjvwSm9uqqjXmgQObDps/d1\n" \
  "OUfJ2wptwsCfLDnmRXulS/bAcbVVOjBIRMx/N1z6Zampp6k4HHiPL1vuFD4UssBE\n" \
  "95A85Ia8XThqUrH8r41MSFEVZA4CNOP43HRUz4ZXVTDtb3a2St2WzQl/Er61rAmj\n" \
  "ePloWFLLG0OQmIPTd6Zpoxeg3JcUiA3BrO3r0nF9rPalZUpWtyG4wqVtEZAgbvSH\n" \
  "PBA=\n" \
  "-----END CERTIFICATE REQUEST-----\n";

const uint32_t SLEEPTIME = 10e6;

const int httpsPort[] PROGMEM = {443};

const char fHost[] PROGMEM = "www.pobanion.com";//instead of pulling from progmem could use macro: f("string")
const char url[] = "/api/plant/addtodb";
const char fFingerprint[] PROGMEM = "A7:72:52:B4:7A:AA:65:A7:49:B2:56:30:01:9C:9D:9A:8B:9C:AF:3C";

const bool debug = true;
//TODO can add wifimanager library to handle initial wifi config

void setup(){
	//turn off wifi until we collect data
	wifiOff();

	if( debug ){
		Serial.begin( 74880 );
		//Serial.setDebugOutput(true);
		Serial.println( "We are in" );
	}

	//Now enable wifi to send data
	wifiOn();
	wifiConnect();
	//Save wifi session in RC
	writeToRtc();
/*
	//Already connected to internet
	char *host = new char[32];
	rfFlash(fHost, host);

	// WiFi.printDiag(Serial);
	Serial.print(F("Connecting"));
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println();
	Serial.print(F("Connected, IP address: "));
	Serial.println(WiFi.localIP());

	// Use WiFiClientSecure class to create TLS connection
	WiFiClientSecure client;
	Serial.print(F("connecting to "));
	Serial.println(host);
//---------------------TODO need to set up sessions for the tcp connection
/* use
BearSSL::WiFiClientSecure.setSession(&BearSSLSession)
before
BearSSL::WiFiClientSecure.connect()
for TLS session
*/
 // Serial.printf("Using fingerprint '%s'\n", fFingerprint);
  //client.setCACert(root_ca);//TODO running into issues here, not sure what I am doing with the cert
  //TODO enable from here
	/*client.setFingerprint(fFingerprint);
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

  WiFi.disconnect( true );
  delay(1);
  ESP.deepSleep( SLEEPTIME, WAKE_RF_DISABLED);//TODO deepsleep
}

void loop() {
  //readTemp(A0);
  //Serial.println("test");
  delay(1000);
}
